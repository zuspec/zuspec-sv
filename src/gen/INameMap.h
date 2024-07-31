/**
 * INameMap.h
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
#include "vsc/dm/IDataType.h"
#include "zsp/arl/dm/IDataTypeFunction.h"

namespace zsp {
namespace sv {
namespace gen {

class INameMap;
using INameMapUP=vsc::dm::UP<INameMap>;
class INameMap {
public:

    enum class Kind {
        Mangled,
        Hierarchical
    };

    virtual ~INameMap() { }

    virtual bool hasName(
        vsc::dm::IAccept    *type,
        Kind                kind=Kind::Mangled) = 0;

    virtual void setName(
        vsc::dm::IAccept    *type,
        const std::string   &name,
        Kind                kind=Kind::Mangled) = 0;

    virtual std::string getName(
        vsc::dm::IAccept    *type,
        Kind                kind=Kind::Mangled) = 0;

};

} /* namespace gen */
} /* namespace sv */
} /* namespace zsp */


