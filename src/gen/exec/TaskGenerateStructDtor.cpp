/*
 * TaskGenerateStructDtor.cpp
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
#include "TaskGenerateStructDtor.h"
#include "TaskIsRefCountField.h"
#include "TaskHasRefCountFields.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateStructDtor::TaskGenerateStructDtor(
    TaskGenerate        *gen,
    IOutput             *out) : m_dbg(0), m_gen(gen), m_out(out) {
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateStructDtor", gen->getDebugMgr());
}

TaskGenerateStructDtor::~TaskGenerateStructDtor() {

}

void TaskGenerateStructDtor::generate(vsc::dm::IDataTypeStruct *t) {
    DEBUG_ENTER("generate");

    m_out->println("virtual function void drop();");
    m_out->inc_ind();
    for (std::vector<vsc::dm::ITypeFieldUP>::const_iterator
        it=t->getFields().begin();
        it!=t->getFields().end(); it++) {
        (*it)->accept(m_this);
    }
    m_out->dec_ind();
    m_out->println("endfunction");

    DEBUG_LEAVE("generate");
}

void TaskGenerateStructDtor::visitDataTypeAddrHandle(arl::dm::IDataTypeAddrHandle *t) {
    m_out->println("%s.drop();", m_field->name().c_str());
}

void TaskGenerateStructDtor::visitDataTypeStruct(vsc::dm::IDataTypeStruct *t) {
    if (TaskHasRefCountFields().check(t)) {
        m_out->println("%s.drop();", m_field->name().c_str());
    }
}

void TaskGenerateStructDtor::visitTypeFieldPhy(vsc::dm::ITypeFieldPhy *f) {
    DEBUG_ENTER("visitTypeFieldPhy");
    m_field = f;
    f->getDataType()->accept(m_this);
    DEBUG_LEAVE("visitTypeFieldPhy");
}

void TaskGenerateStructDtor::visitTypeFieldRef(vsc::dm::ITypeFieldRef *f) {
    m_field = f;
    f->getDataType()->accept(m_this);
}

}
}
}
}
