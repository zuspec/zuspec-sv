/*
 * TaskGenerateCompCheck.cpp
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
#include "TaskGenerateCompCheck.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateCompCheck::TaskGenerateCompCheck(
    TaskGenerate        *gen,
    IOutput             *out) : m_gen(gen), m_out(out) {
    DEBUG_INIT("zsp::sv::gen::exec::TaskGeneateCompCheck", gen->getDebugMgr());
}

TaskGenerateCompCheck::~TaskGenerateCompCheck() {

}

void TaskGenerateCompCheck::generate(vsc::dm::IDataTypeStruct *t) {
    m_out->println("virtual function bit check();");
    m_out->inc_ind();
    m_out->println("bit ret = 1;");
    for (std::vector<vsc::dm::ITypeFieldUP>::const_iterator
        it=t->getFields().begin();
        it!=t->getFields().end(); it++) {
        (*it)->accept(m_this);
    }
    m_out->println("return ret;");
    m_out->dec_ind();
    m_out->println("endfunction");
}

void TaskGenerateCompCheck::visitDataTypeComponent(arl::dm::IDataTypeComponent *t) {

    m_out->println("ret &= %s.check();", m_field->name().c_str());
}

void TaskGenerateCompCheck::visitTypeFieldPhy(vsc::dm::ITypeFieldPhy *f) {
    m_field = f;
    f->getDataType()->accept(m_this);
}

void TaskGenerateCompCheck::visitTypeFieldRef(vsc::dm::ITypeFieldRef *f) {

}

void TaskGenerateCompCheck::visitTypeFieldRegGroup(arl::dm::ITypeFieldRegGroup *f) {
    m_out->println("if (%s == null) begin", f->name().c_str());
    m_out->inc_ind();
    m_out->println("$display(\"Error: register handle not set for %s\");",
        f->name().c_str());
    m_out->println("ret = 0;");
    m_out->dec_ind();
    m_out->println("end");
}

dmgr::IDebug *TaskGenerateCompCheck::m_dbg = 0;

}
}
}
}
