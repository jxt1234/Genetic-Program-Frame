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
#ifndef USER_PACKAGE_H
#define USER_PACKAGE_H
#include <vector>

/*We can not define a destructor function and use freeCallBack for GP_Output, because the freeCallBack is called by the system outside*/
#pragma pack(push)
#pragma pack(4)
struct GP_Output
{
    struct GP_Unit
    {
        void* content;
        void(*freeCallBack)(void*);
    };
    std::vector<GP_Unit> output;
    //Function for convinent
    void push(void* content, void(*freeCallBack)(void*))
    {
        GP_Unit p;
        p.content = content;
        p.freeCallBack = freeCallBack;
        output.push_back(p);
    }
    void clear()
    {
        for (int i=0; i<output.size(); ++i)
        {
            if (output[i].freeCallBack)
            {
                output[i].freeCallBack(output[i].content);
            }
        }
        output.clear();
    }
    inline int size() const {return output.size();}
    template <typename T>
    T* get(int n)
    {
        return (T*)(output[n].content);
    }
    void* operator[](int n) const
    {
        return output[n].content;
    }
};

typedef std::vector<void*> GP_Input;


#define GP_OUTPUT_SINGLE_EXIT(x, result) GP_Output x;x.output.push_back(result);return x;

typedef GP_Output(*computeFunction)(GP_Input inputs);
#pragma pack(pop)
#endif
