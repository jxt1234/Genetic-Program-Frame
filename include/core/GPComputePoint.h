/******************************************************************
   Copyright 2015, Jiang Xiao-tang

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
#ifndef CORE_GPCOMPUTEPOINT_H
#define CORE_GPCOMPUTEPOINT_H
#include "head.h"
#include "GPFunctionDataBase.h"
#include "user/GPContents.h"
class GPComputePoint:public RefCount
{
public:
    GPComputePoint(const GPFunctionDataBase::function* f, int n);
    GPComputePoint(const GPFunctionDataBase::function* f, const std::vector<bool>& completeFlags);
    virtual ~GPComputePoint();
    bool vReceive(GPContents* inputs);
    inline bool completed() const {return mComplte;}
    GPContents* vCompute();
private:
    bool _computeCompleteStatus() const;
    const GPFunctionDataBase::function* mF;
    std::vector<bool> mFlags;
    GPContents mCache;
    bool mComplte;
};
#endif