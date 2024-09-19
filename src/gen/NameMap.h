/**
 * NameMap.h
 *
 * Copyright 2022 Matthew Ballance and Contributors
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
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include "zsp/arl/dm/IDataTypeFunction.h"
#include "zsp/arl/dm/impl/VisitorBase.h"
#include "gen/INameMap.h"
#include "vsc/dm/IDataType.h"

namespace zsp {
namespace sv {
namespace gen {

class NameMap;
using NameMapUP=std::unique_ptr<NameMap>;
/**
 * @brief Supports name mangling function and type names
 * 
 */
class NameMap : 
    public virtual INameMap,
    public virtual arl::dm::VisitorBase {
public:
    NameMap();

    virtual ~NameMap();

    virtual bool hasName(
        vsc::dm::IAccept    *type,
        Kind                kind=Kind::Mangled) override;

    virtual void setName(
        vsc::dm::IAccept    *type, 
        const std::string   &name,
        Kind                kind=Kind::Mangled) override;

    virtual std::string getName(
        vsc::dm::IAccept    *type,
        INameMap::Kind      kind=Kind::Mangled) override;

	virtual void visitDataTypeAction(arl::dm::IDataTypeAction *t) override;

	virtual void visitDataTypeComponent(arl::dm::IDataTypeComponent *t) override;

	virtual void visitDataTypeFlowObj(arl::dm::IDataTypeFlowObj *t) override;

	virtual void visitDataTypeActivity(arl::dm::IDataTypeActivity *t) override;

	virtual void visitDataTypeActivitySequence(arl::dm::IDataTypeActivitySequence *t) override {
        visitDataTypeActivity(t);
    }

    virtual void visitDataTypeAddrSpaceTransparentC(arl::dm::IDataTypeAddrSpaceTransparentC *t) override;

    virtual void visitDataTypeArray(vsc::dm::IDataTypeArray *t) override;

	virtual void visitDataTypeFunction(arl::dm::IDataTypeFunction *t) override;

	virtual void visitDataTypeStruct(vsc::dm::IDataTypeStruct *t) override;

	virtual void visitDataTypeWrapper(vsc::dm::IDataTypeWrapper *t) override;

	virtual void visitTypeField(vsc::dm::ITypeField *t) override;

private:
    using NameM=std::map<vsc::dm::IAccept *, std::string>;
    using KindM=std::map<INameMap::Kind, NameM>;


private:
    std::string                 m_name;
    INameMap::Kind              m_kind;
    KindM                       m_name_m;

};

}
}
}


