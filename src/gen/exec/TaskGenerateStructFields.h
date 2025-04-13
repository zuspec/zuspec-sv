/**
 * TaskGenerateStructFields.h
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
#include "dmgr/IDebugMgr.h"
#include "zsp/arl/dm/impl/VisitorBase.h"
#include "gen/IOutput.h"
#include "IGenRefExpr.h"

namespace zsp {
namespace sv {
namespace gen {
namespace exec {

class TaskGenerate;

class TaskGenerateStructFields :
    public virtual arl::dm::VisitorBase {
public:
    TaskGenerateStructFields(
        TaskGenerate        *gen,
        IOutput             *out
    );

    virtual ~TaskGenerateStructFields();

    virtual void generate(vsc::dm::IDataTypeStruct *t);

    virtual void visitDataTypeAction(arl::dm::IDataTypeAction *t) override;

    virtual void visitDataTypeAddrHandle(arl::dm::IDataTypeAddrHandle *t) override;

    virtual void visitDataTypeArray(vsc::dm::IDataTypeArray *t) override;

    virtual void visitDataTypeBool(vsc::dm::IDataTypeBool *t) override;

    virtual void visitDataTypeInt(vsc::dm::IDataTypeInt *t) override;

    virtual void visitDataTypeList(vsc::dm::IDataTypeList *t) override;

    virtual void visitDataTypeString(vsc::dm::IDataTypeString *t) override;

    virtual void visitDataTypeComponent(arl::dm::IDataTypeComponent*t) override;

    virtual void visitDataTypeStruct(vsc::dm::IDataTypeStruct *t) override;

    virtual void visitTypeField(vsc::dm::ITypeField *f) override;

    virtual void visitTypeFieldPhy(vsc::dm::ITypeFieldPhy *f) override;

    virtual void visitTypeFieldRef(vsc::dm::ITypeFieldRef *f) override;

    virtual void visitTypeFieldInOut(arl::dm::ITypeFieldInOut *f) override;

protected:
    std::string qualifiers(vsc::dm::TypeFieldAttr attr);

protected:
    dmgr::IDebug            *m_dbg;
    TaskGenerate            *m_gen;
    IGenRefExpr             *m_genref;
    IOutput                 *m_out;
    vsc::dm::ITypeField     *m_field;
    vsc::dm::ITypeExpr      *m_init;

};

}
}
}
}


