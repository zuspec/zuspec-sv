/*
 * TaskGenerateCompDoInit.cpp
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
#include "TaskGenerateCompDoInit.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateCompDoInit::TaskGenerateCompDoInit(
    TaskGenerate        *gen,
    IOutput             *out) : m_dbg(0), m_gen(gen), m_out(out) {
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateCompDoInit", gen->getDebugMgr());
}

TaskGenerateCompDoInit::~TaskGenerateCompDoInit() {

}

void TaskGenerateCompDoInit::generate_head(vsc::dm::IDataTypeStruct *t) {
    m_out->println("function void do_init(executor_base_c exec_b);");
    m_out->inc_ind();
}

void TaskGenerateCompDoInit::generate(vsc::dm::IDataTypeStruct *t) {
    generate_head(t);

    m_out->println("init_down(exec_b);");
    for (std::vector<vsc::dm::ITypeFieldUP>::const_iterator
        it=t->getFields().begin();
        it!=t->getFields().end(); it++) {
        (*it)->accept(m_this);
    }
    m_out->println("init_up(exec_b);");

    generate_tail(t);
}

void TaskGenerateCompDoInit::generate_tail(vsc::dm::IDataTypeStruct *t) {
    m_out->dec_ind();
    m_out->println("endfunction");
}

void TaskGenerateCompDoInit::visitDataTypeComponent(arl::dm::IDataTypeComponent *t) {
    m_out->println("%s.do_init(exec_b);", m_field->name().c_str());
}

void TaskGenerateCompDoInit::visitTypeField(vsc::dm::ITypeField *f) {
    m_field = f;
    f->getDataType()->accept(m_this);
}

void TaskGenerateCompDoInit::visitTypeFieldRegGroup(arl::dm::ITypeFieldRegGroup *f) {
    m_out->println("%s.do_init(exec_b);", m_field->name().c_str());
}

}
}
}
}
