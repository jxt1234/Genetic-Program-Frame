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
#include "utils/GP_Clock.h"
#include <time.h>
#include "utils/debug.h"

GP_Clock::GP_Clock(int func)
{
    mStart = clock();
    mId = func;
}

GP_Clock::~GP_Clock()
{
    int inter = clock()-mStart;
    printf("Function Id = %d, times = %dms+%dus\n", mId, inter/1000, inter%1000);
}
