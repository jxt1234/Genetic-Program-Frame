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

#include <algorithm>
#include <map>
#include <set>
#include "GPProducerUtils.h"
#include "math/carryArray.h"
#include "math/carryGroup2.h"
using namespace std;
static void _setUpBasicFunction(GPProducerUtils::func* dst, const GPFunctionDataBase::function* src)
{
    GPASSERT(dst->useChildrenInput.size() == src->inputType.size());
    dst->inputs.clear();
    for (int i=0; i<dst->useChildrenInput.size(); ++i)
    {
        if (0 == dst->useChildrenInput[i])
        {
            dst->inputs.push_back(src->inputType[i]);
        }
        else
        {
            dst->childrenInputs.push_back(src->inputType[i]);
        }
    }
    dst->outputs = src->outputType;
    dst->basic = src;
}

static bool _validFunctions(const vector<const GPFunctionDataBase::function*>& functionList)
{
    bool res = true;
    for (auto f : functionList)
    {
        if (f->inputType.size()>0 && f->fixTable.size()>0)
        {
            GPPRINT("Error for function %s, fixTable and inputType should exist only one\n", f->name.c_str());
            res = false;
        }
    }
    return res;
}

static void _setUpFixTable(const vector<const GPFunctionDataBase::function*>& functions, const map<const GPFunctionDataBase::function*, vector<GPProducerUtils::func*>>& funcmap)
{
    for (auto f : functions)
    {
        if (f->fixTable.size()<=0)
        {
            continue;
        }
        GPASSERT(f->inputType.size() == 0);
        std::vector<std::vector<GPProducerUtils::func*>> tables;
        for (auto pair : f->fixTable)
        {
            /*Expand, */
            carryGroup2<GPProducerUtils::func*> group;
            for (int i=0; i<pair.size(); ++i)
            {
                group.mBase.push_back(funcmap.find(pair[i])->second);
            }
            group.reset();
            do
            {
                tables.push_back(group.current());
            }while (group.next());
        }
        auto pflist = funcmap.find(f)->second;
        for (auto pf : pflist)
        {
            pf->tables = tables;
        }
    }
}

GPProducerUtils::GPProducerUtils(const GPFunctionDataBase* base)
{
    auto functions = base->getAllFunctions();
    GPASSERT(_validFunctions(functions));
    map<FUNC, vector<func*>> funcmap;
    vector<GPPtr<func>> allfunctions;
    /*Create basic function, expand functions*/
    for (auto f : functions)
    {
        if (f->inputType.size()<=0)
        {
            GPPtr<func> mapfunc = new func;
            mapfunc->useChildrenInput = vector<int>(f->inputType.size(), 0);
            _setUpBasicFunction(mapfunc.get(), f);
            allfunctions.push_back(mapfunc);
            vector<func*> list(1, mapfunc.get());
            funcmap.insert(make_pair(f, list));
            continue;
        }
        carryArray array((int)(f->inputType.size()), 2);
        vector<func*> list;
        while (!array.reachEnd())
        {
            GPPtr<func> mapfunc = new func;
            mapfunc->useChildrenInput = array.getCurrent();
            _setUpBasicFunction(mapfunc.get(), f);
            allfunctions.push_back(mapfunc);
            list.push_back(mapfunc.get());
            ++array;
        }
        funcmap.insert(make_pair(f, list));
    }
    /*Set up fix table*/
    _setUpFixTable(functions, funcmap);
    /*Assign Input, and filter invalid functions*/
    mFunctions.clear();
    for (auto f : allfunctions)
    {
        bool valid = true;
        do
        {
            if (f->childrenInputs.size()<=0)
            {
                break;
            }
            GPASSERT(f->tables.empty());
            carryGroup2<func*> group;
            for (int i=0; i<f->childrenInputs.size(); ++i)
            {
                auto pairs = _getFunctionsForOutput(f->childrenInputs[i], allfunctions);
                if (pairs.empty())
                {
                    valid = false;
                    break;
                }
                group.mBase.push_back(pairs);
            }
            if (!valid)
            {
                break;
            }
            do
            {
                f->tables.push_back(group.current());
            } while(group.next());
        }while(0);
        if (valid)
        {
            mFunctions.push_back(f);
        }
    }
}
GPProducerUtils::~GPProducerUtils()
{
}


vector<GPProducerUtils::func*> GPProducerUtils::getFunctionsForOutput(TYPE t) const
{
    return _getFunctionsForOutput(t, mFunctions);
}
vector<GPProducerUtils::func*> GPProducerUtils::_getFunctionsForOutput(TYPE t, const std::vector<GPPtr<func>>& lists) const
{
    vector<GPProducerUtils::func*> result;
    for (auto f : lists)
    {
        if (f->outputs.size() == 1 && t == f->outputs[0])
        {
            result.push_back(f.get());
        }
    }
    return result;
}
