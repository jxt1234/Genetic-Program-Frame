#include "test/GPTest.h"
#include "frontend/GPFormulaTree.h"
#include "frontend/GPFunctionFrontEndProducer.h"
#include "backend/GPBasicPiecesFunctionCreator.h"
#include "backend/GPSingleParallelMachine.h"
#include "core/GPFactory.h"
#include "core/GPProducer.h"
#include "core/GPFunctionTree.h"
#include <iostream>
#include <sstream>
#include "core/GPPieceFactory.h"
#include <fstream>
#include "core/GPStreamFactory.h"
#include "platform/system_lib.h"
using namespace std;


static const char* gFormula =
"REDUCE((MAP((x0), S(x0), [a0]->[a0]), MAP((x1), S(x0), [a0]->[a0])), C(C(x0, y0), x1), [a0, b0]->[1], a0==b0)";

static void _saveOutputPieces(GPPieces* output, const char* prefix)
{
    GPASSERT(output->nKeyNumber <= 1);
    unsigned int n = 1;
    if (output->nKeyNumber == 1)
    {
        n = output->pKeySize[0];
    }
    for (int i=0; i<n; ++i)
    {
        unsigned int key = i;
        GPContents* c = output->vLoad(&key, output->nKeyNumber);
        GPASSERT(c->size() == 1);
        std::stringstream os;
        os << "output/GPBasicParalleCreator"<<prefix << "_"<<i+1<<".jpg";
        GPPtr<GPWStreamWrap> outputStream = GPStreamFactory::NewWStream(os.str().c_str());
        c->getType(0)->vSave(c->get(0), outputStream.get());
        c->decRef();
    }
}


static void __run()
{
    GPPtr<GPFunctionDataBase> base = GPFactory::createDataBase("func.xml", NULL);
    {
        GPPtr<GPProducer> totalProducer = GPFactory::createProducer(base.get());
        GPPtr<GPFunctionFrontEndProducer> producer = new GPFunctionFrontEndProducer(base.get());
        GPPtr<GPFunctionTree> tree = producer->vCreateFromFormula(gFormula, std::vector<const IStatusType*>());
        //FUNC_PRINT_ALL(tree->dump().c_str(), s);
        GPSingleParallelMachine machine;
        GPBasicPiecesFunctionCreator creator(totalProducer.get());
        auto function = creator.vCreateFromFuncTree(tree.get(), &machine);
        GPPieces* inputs = GPPieceFactory::createLocalFilePiece(std::vector<const IStatusType*>{base->vQueryType("TrBmp")}, "res/pictures/", 0);
        inputs->pKeySize[0] = 5;
        inputs->nKeyNumber = 1;
        GPPieces* inputAll[2];
        inputAll[0] = inputs;
        inputAll[1] = inputs;
        GPPieces* outputs = function->vRun(inputAll, 2);
        _saveOutputPieces(outputs, "Compose");
        inputs->decRef();
        outputs->decRef();
        
        delete function;
    }
}


class GPBasicPiecesFunctionCreatorTest:public GPTest
{
    public:
        virtual void run();
        GPBasicPiecesFunctionCreatorTest(){}
        virtual ~GPBasicPiecesFunctionCreatorTest(){}
};
void GPBasicPiecesFunctionCreatorTest::run()
{
    __run();
}

static GPTestRegister<GPBasicPiecesFunctionCreatorTest> a("GPBasicPiecesFunctionCreatorTest");
