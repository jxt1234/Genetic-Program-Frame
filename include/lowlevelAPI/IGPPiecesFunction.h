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
#ifndef LOWLEVELAPI_GPPIECESFUNCTION_H
#define LOWLEVELAPI_GPPIECESFUNCTION_H
#include "GPPieces.h"
#include "IParallelMachine.h"
class IGPPiecesFunction : public GPRefCount
{
public:
    virtual bool vRun(GPPieces** outputs, int outputNumber, GPPieces** inputs, int inputNumber, GPContents* parameters) = 0;

    virtual ~IGPPiecesFunction() {}
protected:
    IGPPiecesFunction() {}
};
#endif