/******************************************************************
 Copyright 2016, Jiang Xiao-tang
 
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
#ifndef USER_GPPARALLELTYPE_H
#define USER_GPPARALLELTYPE_H
#include "GPContents.h"
#include "IGPFunction.h"
class GPSingleTree;

/*Create By Renascence, Used by low level lib*/
struct GPParallelType
{
    IGPFunction* pFunc;
    
    /*For Reconstruct*/
    std::string sFuncFormula;
    std::string sFuncParameter;

    GPSingleTree* pCondition;
    bool (*pConditionRunProc)(GPSingleTree* tree, double* values, unsigned int valueNumber);
    /*For Reconstruct*/
    std::string sConditionFormula;

    //Currently Not used
    std::vector<std::vector<unsigned int>> mSplitInfo;
    
    //a0: 0,0  b2: 1,2  d2: 3,2  and so on
    std::vector<std::pair<unsigned int, unsigned int>> mOutputKey;
    
    
    GPParallelType()
    {
        initialize();
    }
    
    ~GPParallelType()
    {
    }
    
    void initialize()
    {
        pFunc = NULL;
        pCondition = NULL;
    }

};

#endif
