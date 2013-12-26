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
#include "xml/xmlReader.h"
#include <list>
#include <iostream>
#include <fstream>
#include <assert.h>

using namespace std;

xmlReader::xmlReader()
{
    mAttributes = NULL;
    mCurPackage = NULL;
}
xmlReader::~xmlReader(){clear();}

void xmlReader::clear()
{
    list<xmlReader::package*> queue;
    queue.push_back(mAttributes);
    while(!queue.empty())
    {
        xmlReader::package* cur = *(queue.begin());
        if (NULL!=cur)
        {
            for (int j=0; j<cur->children.size(); ++j)
            {
                queue.push_back(cur->children[j]);
            }
            delete cur;
        }
        queue.erase(queue.begin());
    }
    mAttributes = NULL;
    mCurPackage = NULL;
    mString.clear();
    this->subClear();
}

XMLAPI void xmlReader::loadStream(istream& input)
{
    clear();
    loadPackage(input);
}

XMLAPI void xmlReader::loadFile(const char* file)
{
    if (NULL==file) return;
    ifstream read;
    read.open(file, ios::in);
    assert(!read.fail());
    clear();
    loadPackage(read);
    this->attributeUnflatten();
    read.close();
}


bool xmlReader_endPackage(const string& line, int pos)
{
    bool result =  line.find("</")!=string::npos;
    return result;
}

bool xmlReader_startPackage(const string& line, int& cur)
{
    bool result = true;
    cur = line.find("<", cur);
    if (string::npos == cur || cur >=line.size())
    {
        cur = 0;
        result = false;
    }
    else
    {
        // "</"  Means End
        if (line[cur+1]=='/')
        {
            cur = 0;
            result = false;
        }
    }
    //cout <<line << " start "<<result<<endl;
    return result;
}

string xmlReader_getPackageName(const string& line, int& sta)
{
    int cur = sta;
    string result;
    while (line[cur]!='>' && cur < line.size()) cur++;
    if (cur>=line.size()) return result;
    result = line.substr(sta+1, cur-sta-1);
    sta = cur+1;
    return result;
}


string xmlReader_getAttribute(const string& line, int& pos)
{
    int newPos = line.find("</", pos);
    if (string::npos == newPos)
    {
        newPos = line.size();
    }
    string result = line.substr(pos, newPos-pos);
    pos = newPos;
    return result;
}

bool xmlReader_divider(char c)
{
    return (c == ' ') || (c == '\t')||(c == ',');
}

vector<string> xmlReader_divideString(const string& line)
{
#define LENGTH_OUT(x) if (x >= line.size()) {break;}
    vector<string> result;
    int cur =0;
    while(true)
    {
        for (;xmlReader_divider(line[cur]) && cur < line.size();++cur);
        LENGTH_OUT(cur);
        int sta = cur;
        for (++cur;(!xmlReader_divider(line[cur]))&& cur < line.size();++cur);
        int fin = cur;
        result.push_back(line.substr(sta, fin-sta));
        LENGTH_OUT(cur);
        ++cur;
    }
#undef LENGTH_OUT
    return result;
}

void xmlReader::analysisLine(const string& line)
{
    string results = "";
    if (line.size()<=0) return;
    int cur = 0;
    if (xmlReader_startPackage(line, cur))
    {
        string name = xmlReader_getPackageName(line, cur);
        xmlReader::package* newpackage = new xmlReader::package;;
        newpackage->name = name;
        if (NULL!=mCurPackage)
        {
            (mCurPackage->children).push_back(newpackage);
        }
        //Save current string to current package
        vector<string> current = xmlReader_divideString(mString);
        for (int i=0; i<current.size();++i) mCurPackage->attr.push_back(current[i]);
        mString.clear();
        //Turn to new package
        mCurPackage = newpackage;
        mPackage.push_back(mCurPackage);
    }
    //Get attribute
    mString = mString + xmlReader_getAttribute(line, cur);
    //Just if end meet
    if (xmlReader_endPackage(line, cur))
    {
        //Save current string to current package
        vector<string> current = xmlReader_divideString(mString);
        for (int i=0; i<current.size();++i) mCurPackage->attr.push_back(current[i]);
        mString.clear();
        //Turn to new package
        mPackage.pop_back();
        if (!mPackage.empty())
        {
            mCurPackage = *(mPackage.rbegin());
        }
    }
}

xmlReader::package* xmlReader::loadPackage(istream& input)
{
    string tempLine;
    while(getline(input, tempLine, '\n'))
    {
        analysisLine(tempLine);
    }
    mAttributes = mCurPackage;
}

void xmlReaderTest::print()
{
    printUnit(mAttributes);
}
void xmlReaderTest::printUnit(xmlReader::package* p)
{
    if (NULL == p) return;
    cout << endl<<"Name = "<<p->name <<endl;
    cout << "attribute list has :"<<p->attr.size()<<" elements"<<endl;
    const vector<string>& attr = p->attr;
    for (int i=0; i<attr.size(); ++i)
    {
        cout << "The " << i << "attribute:" <<attr[i]<<endl;
    }
    cout << "Has "<<p->children.size()<<" children"<<endl;
    for (int i=0; i< p->children.size(); ++i)
    {
        printUnit(p->children[i]);
    }
}
