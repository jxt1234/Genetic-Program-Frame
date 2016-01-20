%module RenascenceBasic
%{
#include "user/GPAPI.h"
%}
    /*
     * The lib will be loaded  from basic_path/
     * For Example, set basic_path as /a/b/c/
     * Then will load liba,so from /a/b/c/liba.so
     * */
    void GP_Set_Lib_Path(const char* basic_path);

    /*Set Basic path for File Stream, Affect the API GP_Stream_Create and GP_WStream_Create
     * Same as GP_Set_Lib_Path
     * */
    void GP_Set_Stream_Path(const char* basic_path);

    /*Create Read Stream for file, using rb mode. The Real file is basic_path + filename*/
    GPStream* GP_Stream_Create(const char* filename);
    /*Destroy the stream created by GP_Stream_Create, must not be used to destroy other Stream*/
    void GP_Stream_Destroy(GPStream* s);


    /*Create Write Stream for file, using wb mode. The Real file is basic_path + filename*/
    GPWStream* GP_WStream_Create(const char* filename);
    /*Destroy the stream created by GP_WStream_Create, must not be used to destroy other Stream*/
    void GP_WStream_Destroy(GPWStream* s);

    /* Load Contents by IStatusType's vLoad method, Cona AGPContents
     * producer: the FunctionProducer
     * inputs: the list of Stream, must assume that all input are not NULL
     * typeNames: the list of types
     * n: select number, must be equal to types
     * For Example:
     *      AGPContents* c = GP_Contents_Load(producer, inputs, "TrBmp TrBmp", 2);
     */
    AGPContents* GP_Contents_Load(AGPProducer* producer, GPStream** inputs, const char* typeNames, int n);

    /*Return the number of contents*/
    int GP_Contents_Size(AGPContents* contents);


    /*Return the content in the n pos*/
    void* GP_Contents_Get(AGPContents* contents, int n);

    /*Return the double value of GP, must assert that the type is double*/
    double GP_Contents_GetDouble(AGPContents* contents, int n);

    /*Set the double value of GP, must assert that the type is double*/
    void GP_Contents_SetDouble(AGPContents* contents, double value, int n);

    /* Write Contents to appointed stream
     * content: the contents to be write, must not be null
     * outputs: the array of output stream, must not be null, all stream in this list should not be null, too
     * n: the pos of content, must be the range [0, size)
     * Example:
     *      int n = GP_Contents_Size(contents);
     *      for (int i=0; i<n; ++i)
     *      {
     *          char text[20];
     *          sprintf(text, "test%d.txt", i);
     *          outputStream = GP_WStream_Create(text);
     *          GP_Contents_Save(contents, outputStream, i);
     *          GP_WStream_Destroy(outputStream);
     *      }
     */
    void GP_Contents_Save(AGPContents* content, GPWStream* outputs, int n);

    /*Destroy the contents in AGPContents and itself
     * content: The memory to be freed
     */
    void GP_Contents_Destroy(AGPContents* content);


    /* Create a collector, used to merge contents, the collector is also needed to be free by GP_Contents_Destroy, the collector doesn't own its content*/
    AGPContents* GP_Contents_CreateCollector();

    /*Select the nth content in B, and add to Collector's end. User should not destroy B before collector is destroyed*/
    /* Collector: The Dst Container, Must be come from GP_Contents_CreateCollector
     * B: The Src
     * n: The pos of content in B, n should be >=0, and n < B->size
     * Example:
     *      AGPContents* col = GP_Contents_CreateCollector();
     *      AGPContents* B = GP_Contents_Load(producer, inputsB, "TrBmp", 1);
     *      AGPContents* A = GP_Contents_Load(producer, inputsA, "TrBmp TrBmp", 2);
     *      GP_Contents_Collect(col, A, 0);
     *      GP_Contents_Collect(col, A, 1);
     *      GP_Contents_Collect(col, B, 0);
     *      //Do Something by col
     *      GP_Contents_Destroy(col);
     *      GP_Contents_Destroy(A);
     *      GP_Contents_Destroy(B);
     */
    void GP_Contents_Collect(AGPContents* Collector, AGPContents* B, int n);




    /*Create AGPProducer by function table, meta file and type
     * metaStream:
     * table: obtain the method to get function, can be NULL, then GP will create table by the path decribed by meta file
     * n: the number of stream and tables
     * type: ALL:-1, TREE:0, GRAPIC:1, STREAM:2
     */
    AGPProducer* GP_Producer_Create(GPStream** metaStream, IFunctionTable** table, int n, int type);

    /*Destroy the AGPProducer, before using this API, Make sure that all function created by p is destroyed*/
    void GP_Producer_Destroy(AGPProducer* p);


    typedef struct GPOptimizorInfo
{
    /*The Max Tree Depth For all ADF Function
     * For Example:
     * If The nMaxADFDepth is set as 1, then ADF(x0, x1) can only be f(x0, x1), can't be f(g(x0), x1)
     */
    int nMaxADFDepth;

    /*Determine which method for optimization. Make different sense for structure and parameter*/
    int nOptimizeType;

    /*The Function to Determine whether the Auto-Generate-Function is good, the larger the better*/
    double(*pFitComputeFunction)(IGPAutoDefFunction* adf, void* pMeta);
    void* pMeta;

    /*The max time for pFitComputeFunction to call in optimization*/
    int nMaxRunTimes;
        double fTargetBestValue;
        GPWStream* pBestInfo;
}GPOptimizorInfo;

/*Create a IGPAutoDefFunction by inputTypes and outputTypes, the types must be divided by space*/
/*For example: IGPAutoDefFunction* f = GP_Create_Function_ByType(p, "SkBitmap SkPaint", "SkStream", false)*/
/*
 * p: the AGPProducer created by GP_Producer_Create
 * outputTypes: the name of all output types
 * inputTypes: all the input types obtained for the IGPAutoDefFunction, not orderred, if set "", GP will not care about the input
 */
IGPAutoDefFunction* GP_Function_Create_ByType(const AGPProducer* p, const char* outputTypes, const char* inputTypes, GPOptimizorInfo* pInfo);

/*Create GP by formula like this: f(g(a, b), c)
 * p: the AGPProducer created by GP_Producer_Create
 * formula: the string of formula
 * inputType: the string of all types
 * pInfo：The Optimization info, only valid where formula contains ADF
 * Example: auto adf = GP_Function_Create_ByFormula(p, "f(x0, g(x1))", "Matrix Matrix", NULL);
 */
IGPAutoDefFunction* GP_Function_Create_ByFormula(const AGPProducer* p, const char* formula, const char* inputType, GPOptimizorInfo* pInfo);

/*The Inputs should be generate from stream by IStatusType inorder by the inputTypes return from GP_Function_Get_Inputs*/
AGPContents* GP_Function_Run(IGPAutoDefFunction* f, AGPContents* input);
/*Free the memory of function*/
void GP_Function_Destroy(IGPAutoDefFunction* f);

/*Load f from input stream, which mainly come from a xmlfile*/
IGPAutoDefFunction* GP_Function_Create_ByStream(const AGPProducer* p, GPStream* xmlFile);

/*Save f to output, use xml format*/
void GP_Function_Save(IGPAutoDefFunction* f, GPWStream* output);

/*Optimize IGPAutoDefFunction by adjust its parameters*/
/*REMIND: the f's parameters will be changed after this api*/
/*If fit_fun is not random, this api make sure that fit_fun(f) will at least not decrease*/
/*
f: The IGPAutoDefFunction to be Optimize
pInfo: obtain function to determine f's fit, nMaxRunTimes is the maxTimes to run IGPAutoDefFunction in these api, will affect the algorithm of PSO. The nMaxADFDepth make no sense
pInfo.nOptimizeType: 0 (use PSO, will compute maxTimes), 1(Use random golden divide Optimization, fast but not very accuracy)

example: 
{
struct GPOptimizorInfo info;
info.nMaxRunTimes = 1000;
info.pFitComputeFunction = f;
info.pMeta = meta;
GP_Function_Optimize(f, &info);
}
 */
void GP_Function_Optimize(IGPAutoDefFunction* f, GPOptimizorInfo* pInfo);


/*For the convenience of Python/Go API*/
GPStream** GP_Streams_Malloc(int n);
void GP_Streams_Free(GPStream** streams);
GPStream* GP_Streams_Get(GPStream** streams, int n);
void GP_Streams_Set(GPStream** streams,GPStream* contents, int n);

/*For Print*/
typedef struct AGPStrings AGPStrings;
int GP_Strings_Number(AGPStrings* strings);
const char* GP_Strings_Get(AGPStrings* strings, int n);
void GP_Strings_Free(AGPStrings* strings);

/*AGPStrings come from function must be free by GP_Strings_Free*/
AGPStrings* GP_Function_GetFormula(IGPAutoDefFunction* f, const char* name);
AGPStrings* GP_Function_GetParameters(IGPAutoDefFunction* f);
AGPStrings* GP_Producer_ListFunctions(AGPProducer* producer);
AGPStrings* GP_Producer_ListTypes(AGPProducer* producer);

/*Return all type name of contents*/
AGPStrings* GP_Contents_Types(AGPContents* contents);


/*Default GPOptimizorInfo*/
enum
{
    GP_OPTIMIZOR_VALUE = 0,
    GP_OPTIMIZOR_TIME = 1
};
/*Create standard OptimizorInfo, Used by Python/Go
 *type: GP_OPTIMIZOR_VALUE/GP_OPTIMIZOR_TIME
 *depth, maxtimes: the value in GPOptimizorInfo
 */
GPOptimizorInfo* GP_OptimzorInfo_CreateTemplate(int depth, int maxtimes, int type, AGPContents* pInput, GPWStream* bestCache, IGPAutoDefFunction* postFunction);

/*The info be freed must be come from GP_OptimzorInfo_CreateTemplate and can't be modified*/
void GP_OptimzorInfo_FreeTemplate(GPOptimizorInfo* pInfo);
AGPContents* GP_Contents_CreateDouble(double value);

void GP_Function_MapParameters(IGPAutoDefFunction* f, const char* parameters);

