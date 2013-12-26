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
#ifndef CORE_IGPAUTODEFFUNCTION_H
#define CORE_IGPAUTODEFFUNCTION_H
#include "IDataBase.h"
#include "function.h"
class IGPAutoDefFunction
{
    public:
        inline GP_Output run(GP_Input inputs)
        {
            int cur = 0;
            this->input(inputs, cur);
            this->compute(mFuncDataBase);
            return this->output();
        }
        inline void setDataBase(IRuntimeDataBase* func){mFuncDataBase = func;}
        /*The combined function is implement by this circle: input--compute--output */
        //The input function may delete the content of inp, so the inp must be reconstruct after input 
        virtual void input(GP_Input& inp, int& cur)=0;
        virtual void compute(IRuntimeDataBase* map)=0;
        virtual GP_Output output() = 0;
        //Basic Function
        IGPAutoDefFunction(){mFuncDataBase = NULL;}
        virtual ~IGPAutoDefFunction(){}
    private:
        IRuntimeDataBase* mFuncDataBase;
};
#endif
