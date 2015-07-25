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
#ifndef USER_GPSTREAM_H
#define USER_GPSTREAM_H
#include <stdlib.h>
struct GPStream
{
    /*If the buffer is NULL, it means skip size bytes, return the fact bytes it read*/
    size_t(*mRead)(void* meta, void* buffer, size_t size);
    /*Return true if the stream has moved to end*/
    bool(*mIsEnd)(void* meta);
    
    size_t read(void* buffer, size_t size) const
    {
        return mRead(mMetaData, buffer, size);
    }
    
    bool isEnd() const
    {
        return mIsEnd(mMetaData);
    }
    
    template <typename T>
    T readUnit()
    {
        T buffer;
        this->mRead(mMetaData, &buffer, sizeof(T));
        return buffer;
    }
    void* mMetaData;
    
    bool isValid() const
    {
        return NULL!=mMetaData && NULL!=mRead && NULL!=mIsEnd;
    }
};
struct GPWStream
{
    size_t(*mWrite)(void* meta, const void* buffer, size_t size);
    bool(*mFlush)(void* meta);
    size_t write(const void* buffer, size_t size)
    {
        return mWrite(mMetaData, buffer, size);
    }
    bool flush()
    {
        return mFlush(mMetaData);
    }
    
    template<typename T>
    bool writeUnit(T v)
    {
        return this->mWrite(mMetaData, &v, sizeof(T));
    }
    void* mMetaData;
    
    bool isValid() const
    {
        return NULL!=mWrite && NULL!=mFlush && NULL!=mMetaData;
    }
};
#endif
