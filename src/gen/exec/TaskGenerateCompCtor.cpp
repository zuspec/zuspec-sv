/*
 * TaskGenerateCompCtor.cpp
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
#include "dmgr/impl/DebugMacros.h"
#include "TaskGenerate.h"
#include "TaskGenerateCompCtor.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateCompCtor::TaskGenerateCompCtor(
    TaskGenerate        *gen,
    IOutput             *out) : TaskGenerateStructCtor(gen, out) {
    m_dbg = 0;
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateCompCtor", gen->getDebugMgr()); 
}

TaskGenerateCompCtor::~TaskGenerateCompCtor() {

}

void TaskGenerateCompCtor::generate_head(vsc::dm::IDataTypeStruct *t) {
    m_out->println("function new(string name, component_ctor_ctxt ctxt, component_c parent=null);");
    m_out->inc_ind();
    m_out->println("super.new(name, ctxt, parent);");
    m_out->println("");
    m_out->println("if (ctxt != null) begin");
    m_out->inc_ind();
    m_out->println("ctxt.enter(this);");
    m_out->dec_ind();
    m_out->println("end");
}

void TaskGenerateCompCtor::generate_tail(vsc::dm::IDataTypeStruct *t) {
    m_out->dec_ind();
    m_out->println("endfunction");
}

void TaskGenerateCompCtor::visitDataTypeAddrSpaceC(arl::dm::IDataTypeAddrSpaceC *t) {
    m_out->println("%s = new(\"%s\", ctxt, this);",
        m_field->name().c_str(),
        m_field->name().c_str());
    // TODO: register this address space with the context
}

void TaskGenerateCompCtor::visitDataTypeAddrSpaceTransparentC(arl::dm::IDataTypeAddrSpaceTransparentC *t) {
    visitDataTypeAddrSpaceC(t);
}

void TaskGenerateCompCtor::visitDataTypeComponent(arl::dm::IDataTypeComponent *t) {
    m_out->println("%s = new(\"%s\", ctxt, this);",
        m_field->name().c_str(),
        m_field->name().c_str());
}

void TaskGenerateCompCtor::visitTypeFieldRegGroup(arl::dm::ITypeFieldRegGroup *f) {
    m_out->println("%s = new(\"%s\", this);", f->name().c_str(), f->name().c_str());
}

}
}
}
}
