/*
 * TaskGenerateCompInit.cpp
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
#include "TaskGenerateCompInit.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateCompInit::TaskGenerateCompInit(
    TaskGenerate        *gen,
    IOutput             *out) : m_dbg(0), m_gen(gen), m_out(out) {
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateCompInit", gen->getDebugMgr());
}

TaskGenerateCompInit::~TaskGenerateCompInit() {

}

void TaskGenerateCompInit::generate_head(vsc::dm::IDataTypeStruct *t) {
    m_out->println("function void init();");
    m_out->inc_ind();
}

void TaskGenerateCompInit::generate(vsc::dm::IDataTypeStruct *t) {
    generate_head(t);

    m_init_down = true;
    m_out->println("init_down();");
    for (std::vector<vsc::dm::ITypeFieldUP>::const_iterator
        it=t->getFields().begin();
        it!=t->getFields().end(); it++) {
        (*it)->accept(m_this);
    }

    m_init_down = false;
    for (std::vector<vsc::dm::ITypeFieldUP>::const_iterator
        it=t->getFields().begin();
        it!=t->getFields().end(); it++) {
        (*it)->accept(m_this);
    }
    m_out->println("init_up();");

    generate_tail(t);
}

void TaskGenerateCompInit::generate_tail(vsc::dm::IDataTypeStruct *t) {
    m_out->dec_ind();
    m_out->println("endfunction");
}

void TaskGenerateCompInit::visitDataTypeComponent(arl::dm::IDataTypeComponent *t) {
    if (m_init_down) {
        m_out->println("%s.init_down();", m_field->name().c_str());
    } else {
        m_out->println("%s.init_up();", m_field->name().c_str());
    }
}

void TaskGenerateCompInit::visitTypeField(vsc::dm::ITypeField *f) {
    m_field = f;
    f->getDataType()->accept(m_this);
}

}
}
}
}
