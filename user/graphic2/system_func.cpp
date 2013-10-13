/*AUTO-GENERATE CODE STYLE*/
#include "user/total.h"
#include <dlfcn.h>
#include <assert.h>
computeFunction* gAllFunctions;
int gAllFunctionsNumber;
void* gFunctionHandle=NULL;
void destroyFunction()
{
    dlclose(gFunctionHandle);
}
void initFunctions()
{
    if(NULL==gFunctionHandle) gFunctionHandle = dlopen("libpics.so", RTLD_NOW);
    assert(NULL!=gFunctionHandle);
    gAllFunctions = new computeFunction[8];
    gAllFunctionsNumber = 8;
    gAllFunctions[0]=(computeFunction)dlsym(gFunctionHandle, "TrPackageInput");
    gAllFunctions[1]=(computeFunction)dlsym(gFunctionHandle, "TrPackageInputTarget");
    gAllFunctions[2]=(computeFunction)dlsym(gFunctionHandle, "TrPackageFitCompute");
    gAllFunctions[3]=(computeFunction)dlsym(gFunctionHandle, "TrPackageFilterMatrix");
    gAllFunctions[4]=(computeFunction)dlsym(gFunctionHandle, "TrPackageCompse");
    gAllFunctions[5]=(computeFunction)dlsym(gFunctionHandle, "TrPackageFilterMatrixRegress");
    gAllFunctions[6]=(computeFunction)dlsym(gFunctionHandle, "TrPackageFilterTransformFromRegress");
    gAllFunctions[7]=(computeFunction)dlsym(gFunctionHandle, "TrPackageOutput");
}
