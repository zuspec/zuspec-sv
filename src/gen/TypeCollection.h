/**
 * TypeCollection.h
 *
 * Copyright 2023 Matthew Ballance and Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may 
 * not use this file except in compliance with the License.  
 * You may obtain a copy of the License at:
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
 * See the License for the specific language governing permissions and 
 * limitations under the License.
 *
 * Created on:
 *     Author: 
 */
#pragma once
#include <memory>
#include <set>
#include <vector>
#include <unordered_map>
#include "vsc/dm/IDataType.h"

namespace zsp {
namespace sv {
namespace gen {

class TypeCollection;
using TypeCollectionUP=std::unique_ptr<TypeCollection>;
class TypeCollection {
public:
    TypeCollection();

    virtual ~TypeCollection();

    bool addType(vsc::dm::IDataType *t);

    int32_t getTypeId(vsc::dm::IDataType *t);

    void addDep(
        vsc::dm::IDataType      *target,
        vsc::dm::IDataType      *dependent);

    vsc::dm::IDataType *getType(int32_t id);

    std::vector<int32_t> sort();

private:
    std::vector<vsc::dm::IDataType *>                   m_type_l;
    std::unordered_map<vsc::dm::IDataType *, int32_t>   m_type_m;
    std::vector<std::set<int32_t>>                      m_dep_m;

};

}
}
}


