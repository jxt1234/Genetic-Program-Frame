/******************************************************************
   Copyright 2014, Jiang Xiao-tang

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
#include "core/GPData.h"
#include <sstream>

using namespace std;

#define DIVIDE_LITTLE 0.00001

GPData::GPData(const std::string& name):mName(name)
{
}
GPData::~GPData()
{
    for (int i=0; i<mData.size(); ++i)
    {
        delete mData[i];
    }
}

void GPData::addData(void* content, const IStatusType& type)
{
    GPData::data* d = new GPData::data(content, type);
    mData.push_back(d);
}

GP_Input GPData::expand() const
{
    GP_Input res;
    for (int i=0; i<mData.size(); ++i)
    {
        res.push_back(mData[i]->content);
    }
    return res;
}

void GPData::print(std::ostream& out) const
{
    out << "<"<<mName<<">\n";
    for (int i=0; i<mData.size(); ++i)
    {
        GPData::data* d = mData[i];
        out << "<"<<(d->type).name()<<">\n";
        (d->type).print(out, d->content);
        out << "\n</"<<(d->type).name()<<">\n";
    }
    out << "</"<<mName<<">\n";
}
/*Default Compare method, assume that all values is double*/
double GPData::compare(const GP_Output& output)
{
    double res = 0;
    do
    {
        if (output.size()!=mData.size())
        {
            res = 1;
            break;
        }
        for (int i=0; i<mData.size(); ++i)
        {
            GPData::data* d = mData[i];
            const IStatusType& t = d->type;
            ostringstream _data, _out;
            t.print(_data, d->content);
            t.print(_out, output[i]);
            istringstream data(_data.str());
            istringstream out(_out.str());
            bool hasContent = true;
            double _d;
            double _o;
            double sum = 0;
            int n = 0;
            while(hasContent)
            {
                hasContent = data >> _d;
                out >> _o;
                double err = (_o-_d)*(_o-_d);
                double div = (_o*_o+_d*_d+DIVIDE_LITTLE)*2;
                ++n;
                sum += err/div;
            }
            res += sum/n;
        }
        res = res/(double)(mData.size());
    }while(0);
    return res;
}