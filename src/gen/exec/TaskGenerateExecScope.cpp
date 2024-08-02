/*
 * TaskGenerateExecScope.cpp
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
#include "TaskGenerateDataType.h"
#include "TaskGenerateExecScope.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateExecScope::TaskGenerateExecScope(
    TaskGenerate            *gen,
    IOutput                 *out) : m_dbg(0), m_gen(gen), m_out_top(out), m_exec(0) {
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateExecScope", gen->getDebugMgr());
}

TaskGenerateExecScope::~TaskGenerateExecScope() {

}

void TaskGenerateExecScope::generate(
        arl::dm::ITypeProcStmtScope *scope,
        bool                        newscope) {
    OutputExecScope out(newscope, m_out_top);
    m_exec = &out;

    if (newscope) {
        m_out_top->println("begin");
        m_out_top->inc_ind();
    }

    for (std::vector<arl::dm::ITypeProcStmtUP>::const_iterator
        it=scope->getStatements().begin();
        it!=scope->getStatements().end(); it++) {
        (*it)->accept(m_this);
    }

    m_exec->apply(m_out_top);

    if (newscope) {
        m_out_top->dec_ind();
        m_out_top->println("end");
    }
}

void TaskGenerateExecScope::visitTypeProcStmtVarDecl(arl::dm::ITypeProcStmtVarDecl *t) {
    m_exec->decl()->indent();
    TaskGenerateDataType(m_gen, m_exec->decl()).generate(t->getDataType());
    // TODO: must 'new' if an aggregate type
    m_exec->decl()->write(" %s;\n", t->name().c_str());
}
 
}
}
}
}
