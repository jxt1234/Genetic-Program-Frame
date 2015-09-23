/******************************************************************
 Copyright 2013, Jiang Xiao-tang
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 ******************************************************************/
#ifndef CORE_ABSTRACTGP_H
#define CORE_ABSTRACTGP_H
#include <stdlib.h>
#include "utils/AbstractPoint.h"
#include "user/GPContents.h"
#include "core/GPFunctionDataBase.h"
#include "core/IGPAutoDefFunction.h"
#include "core/GPStatusContent.h"
#include "utils/GP_Clock.h"
#include "utils/debug.h"

class GPTreeADFPoint:public AbstractPoint
{
public:
    GPTreeADFPoint();
    GPTreeADFPoint(const GPFunctionDataBase::function* func);
    void initStatus(const std::vector<std::istream*>& is);
    virtual ~GPTreeADFPoint();
    inline const GPFunctionDataBase::function* func() const {return mFunc;}
    inline const std::vector<GPStatusContent*>& status() {return mStatus;}
    inline void pGetInputs(std::vector<const IStatusType*>& tlist) const
    {
        getinput(tlist);
    }
private:
    /*Attributes*/
    const GPFunctionDataBase::function* mFunc;
    std::vector<GPStatusContent*> mStatus;
    /*Function needed*/
    GPTreeNode* xmlPrint() const;
    //The cur means from which element to use, it's assumed that the numbers is valid and the status has been alloc
    GPContents* compute(GPContents* input, int& cur);
    //For recursive
    void getinput(std::vector<const IStatusType*>& tlist) const;
    
    class GPTreeADFCopy:public AbstractPoint::IPointCopy
    {
    public:
        GPTreeADFCopy(){}
        virtual ~GPTreeADFCopy(){}
        virtual AbstractPoint* copy(AbstractPoint* src);
    };
    void _replacePoint(const std::vector<int> &numbers, int& cur, const GPFunctionDataBase* base);
    friend class GPTreeADF;
};

class GPTreeProducer;
class GPTreeADF:public IGPAutoDefFunction
{
public:
    //Basic Functions
    GPTreeADF(GPTreeADFPoint* root, const GPTreeProducer* p);
    virtual ~GPTreeADF();
    virtual IGPAutoDefFunction* vCopy() const;
    //Basic API
    inline GPTreeADFPoint* root() const {return mRoot;}
    GPTreeADFPoint* find(float rate);/*rate must be [0,1), return n*rate's element*/
    virtual GPContents* vRun(GPContents* inputs);
    virtual GPTreeNode* vSave() const;
    virtual int vMap(GPPtr<GPParameter> para);
    virtual void vMutate();
    //FIXME: dangerous api
    static void loadUnitFunction(std::vector<int>& result, int functionId, int statusId, int number);
protected:
    //FIXME Reconstruct the xmlGPLoader
    friend class xmlGPLoader;
private:
    void _refreshInputsAndOutputs();
    GPTreeADFPoint* mRoot;
    const GPTreeProducer* mProducer;
    bool mNeedRefresh;
};


#endif
