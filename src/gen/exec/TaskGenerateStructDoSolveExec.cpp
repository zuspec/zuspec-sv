/*
 * TaskGenerateStructDoSolveExec.cpp
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
#include "TaskGenerateStructDoSolveExec.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateStructDoSolveExec::TaskGenerateStructDoSolveExec(
    TaskGenerate        *gen,
    IOutput             *out) : m_gen(gen), m_out(out) {
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateStructDoSolveExec", 
        gen->getDebugMgr());
}

TaskGenerateStructDoSolveExec::~TaskGenerateStructDoSolveExec() {

}

void TaskGenerateStructDoSolveExec::generate(
    vsc::dm::IDataTypeStruct    *t,
    bool                        executor,
    std::string                 &kind) {
    m_kind = kind;
    m_executor = executor;
    m_out->println("function void do_%s(%s);", 
        kind.c_str(),
        (executor)?"executor_base exec_b":"");
    m_out->inc_ind();
    m_out->println("%s(%s);", 
        kind.c_str(),
        (executor)?"exec_b":"");
    for (std::vector<vsc::dm::ITypeFieldUP>::const_iterator
        it=t->getFields().begin();
        it!=t->getFields().end(); it++) {
        (*it)->accept(m_this);
    }

    m_out->dec_ind();
    m_out->println("endfunction");
}

void TaskGenerateStructDoSolveExec::visitDataTypeAddrHandle(arl::dm::IDataTypeAddrHandle *t) {
    // Address handles don't participate in randomization
}

void TaskGenerateStructDoSolveExec::visitDataTypeStruct(vsc::dm::IDataTypeStruct *t) {
    m_out->println("%s.do_%s(%s);", 
        m_field->name().c_str(),
        m_kind.c_str(),
        (m_executor)?"exec_b":"");
}

void TaskGenerateStructDoSolveExec::visitTypeFieldPhy(vsc::dm::ITypeFieldPhy *f) {
    m_field = f;
    f->getDataType()->accept(m_this);
}

void TaskGenerateStructDoSolveExec::visitTypeFieldRef(vsc::dm::ITypeFieldRef *f) {

}

dmgr::IDebug *TaskGenerateStructDoSolveExec::m_dbg = 0;

}
}
}
}
