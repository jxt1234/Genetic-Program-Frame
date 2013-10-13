/*AUTO-GENERATE CODE STYLE*/
#include "user/total.h"
#include <dlfcn.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
extern void* gFunctionHandle;
typedef void*(*STATUS_ALLOC)();
typedef void(*STATUS_VARY)(void*);
typedef void(*STATUS_COPY)(void*, void*);
typedef std::string(*STATUS_PRINT)(void*);
void destroyStatus()
{
}
vector<int> initStatusType()
{
    vector<int> result;
    if(gFunctionHandle==NULL) gFunctionHandle = dlopen("libpics.so", RTLD_NOW);
    STATUS_ALLOC TrFilterMatrix_alloc = (STATUS_ALLOC)dlsym(gFunctionHandle, "TrFilterMatrix_alloc");
    STATUS_VARY TrFilterMatrix_free = (STATUS_VARY)dlsym(gFunctionHandle, "TrFilterMatrix_free");
    STATUS_VARY TrFilterMatrix_vary = (STATUS_VARY)dlsym(gFunctionHandle, "TrFilterMatrix_vary");
    STATUS_COPY TrFilterMatrix_copy = (STATUS_COPY)dlsym(gFunctionHandle, "TrFilterMatrix_copy");
    STATUS_PRINT TrFilterMatrix_print = (STATUS_PRINT)dlsym(gFunctionHandle, "TrFilterMatrix_print");
    STATUS_ALLOC float_alloc = (STATUS_ALLOC)dlsym(gFunctionHandle, "float_alloc");
    STATUS_VARY float_free = (STATUS_VARY)dlsym(gFunctionHandle, "float_free");
    STATUS_VARY float_vary = (STATUS_VARY)dlsym(gFunctionHandle, "float_vary");
    STATUS_COPY float_copy = (STATUS_COPY)dlsym(gFunctionHandle, "float_copy");
    STATUS_PRINT float_print = (STATUS_PRINT)dlsym(gFunctionHandle, "float_print");
    STATUS_ALLOC TrRegreeMode_alloc = (STATUS_ALLOC)dlsym(gFunctionHandle, "TrRegreeMode_alloc");
    STATUS_VARY TrRegreeMode_free = (STATUS_VARY)dlsym(gFunctionHandle, "TrRegreeMode_free");
    STATUS_VARY TrRegreeMode_vary = (STATUS_VARY)dlsym(gFunctionHandle, "TrRegreeMode_vary");
    STATUS_COPY TrRegreeMode_copy = (STATUS_COPY)dlsym(gFunctionHandle, "TrRegreeMode_copy");
    STATUS_PRINT TrRegreeMode_print = (STATUS_PRINT)dlsym(gFunctionHandle, "TrRegreeMode_print");
    result.push_back(status_allocType(0, TrFilterMatrix_alloc, TrFilterMatrix_free, TrFilterMatrix_vary, TrFilterMatrix_copy, TrFilterMatrix_print));
    result.push_back(status_allocType(0, float_alloc, float_free, float_vary, float_copy, float_print));
    result.push_back(status_allocType(0, TrRegreeMode_alloc, TrRegreeMode_free, TrRegreeMode_vary, TrRegreeMode_copy, TrRegreeMode_print));
    return result;
}
