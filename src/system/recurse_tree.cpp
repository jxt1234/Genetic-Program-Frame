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
#include "system/recurse_tree.h"
#include <list>
#include <algorithm>
#include "system/GenerateSystem.h"
#include "core/AbstractGP.h"
using namespace std;

vector<int> computePoint::filter(const vector<vector<int> >& combo, const vector<int>& output)
{
    vector<int> result;
    for (int i=0; i<combo.size(); ++i)
    {
        const vector<int>& comboUnit = combo[i];
        bool fit = true;
        const vector<int>& input_obtained = mSys->mInputTypeId;
        for (int j=0; j<comboUnit.size(); ++j)
        {
            /*Check if it contained old output*/
            if (find(output.begin(), output.end(), comboUnit[j]) != output.end())
            {
                fit = false;
                break;
            }
            /*Check if it contained unobtained input*/
            const computeSystem::function& f = mSys->getDetailFunction(comboUnit[j]);
            for (int k=0; k<f.inputType.size(); ++k)
            {
                int inpId = f.inputType[k];
                /*TODO break circle outside*/
                if (find(input_obtained.begin(), input_obtained.end(), inpId) == input_obtained.end())
                {
                    fit = false;
                    break;
                }
            }
        }
        if (fit)
        {
            result.push_back(i);
        }
    }
    return result;
}


vector<int> computePoint::getDependOutput()
{
    vector<int> result;
    computePoint* cur = dynamic_cast<computePoint*>(mDepend);
    computePoint* self = this;
    while(NULL!=cur)
    {
        const vector<int>& data = cur->getData();
        result.push_back(data[self->mParent]);
        self = cur;
        cur = dynamic_cast<computePoint*>(cur->mDepend);
    }
    return result;
}
bool computePoint::vGrow()
{
    bool success = true;
    assert(NULL!=mSys);
    const vector<int>& data = getData();
    vector<int> currentOutputId = getDependOutput();
    for (int i=0; i<data.size(); ++i)
    {
        const vector<vector<int> >& inputData = mSys->getAvailableFunctionInputs(data[i]);
        currentOutputId.push_back(data[i]);
        vector<int> avail = filter(inputData, currentOutputId);
        currentOutputId.erase(currentOutputId.end()-1);
        if (!avail.empty())
        {
            carryPoint* res = new computePoint(inputData, avail, mSys);
            computePoint* midres = dynamic_cast<computePoint*>(res);
            midres->mDepend = this;
            midres->mParent = i;
            mChild.push_back(res);
        }
        else if (!inputData.empty())
        {
            //This point is invalid, inform it
            success = false;
            break;
        }
    }
    return success;
}
vector<int> computeSearchTree::output()
{
    computePoint* cur;
    vector<int> result;
    list<carryPoint*> queue;
    queue.push_back(mRoot);
    while(!queue.empty())
    {
        cur = dynamic_cast<computePoint*>(*(queue.begin()));
        //Construct search queue
        for (int i=0; i<cur->mChild.size(); ++i)
        {
            queue.push_back(cur->mChild[i]);
        }
        const vector<int>& data = cur->getData();
        //Construct the inputsNumbers
        vector<int> inputNumber(data.size(),0);
        for (int i=0; i<cur->mChild.size(); ++i)
        {
            computePoint* child = dynamic_cast<computePoint*>(cur->mChild[i]);
            inputNumber[child->mParent] = child->getData().size();
        }
        //Construct queue, without status
        for (int i=0; i<data.size(); ++i)
        {
            AbstractGP::loadUnitFunction(result, data[i], -1, inputNumber[i]);
        }
        queue.erase(queue.begin());
    }
    return result;
}
bool computeSearchTree::readyToOutput()
{
    return true;
}