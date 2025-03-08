/*
 * TaskGenerateExecBlock.cpp
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
#include "TaskGenerateExecBlock.h"
#include "TaskGenerateExecScope.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateExecBlock::TaskGenerateExecBlock(
    TaskGenerate        *gen,
    IGenRefExpr         *genref,
    IOutput             *out) : m_dbg(0), m_gen(gen), m_genref(genref), m_out(out) {
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateExecBlock", gen->getDebugMgr());
}

TaskGenerateExecBlock::~TaskGenerateExecBlock() {

}

void TaskGenerateExecBlock::generate(
    arl::dm::ITypeExecProc *t,
    bool                    istask,
    bool                    executor,
    const std::string       &fname) {

}

void TaskGenerateExecBlock::generate(
        const std::vector<arl::dm::ITypeExecUP> &t,
        bool                                    istask,
        bool                                    executor,
        const std::string                       &fname) {
    DEBUG_ENTER("generate %s", fname.c_str());
    m_out->println("%s %s(%s);", 
        (istask)?"task":"function void", 
        fname.c_str(),
        (executor)?"executor_base_c exec_b":"");
    m_out->inc_ind();
    if (executor) {
        m_out->println("executor_t executor;");
        m_out->println("pss_import_api api;");
        m_out->println("if (!$cast(executor, exec_b)) begin");
        m_out->inc_ind();
        m_out->println("`ZSP_FATAL((\"Failed to cast executor\"));");
        m_out->dec_ind();
        m_out->println("end");
        m_out->println("if (executor == null) begin");
        m_out->inc_ind();
        m_out->println("`ZSP_FATAL((\"executor is null\"));");
        m_out->dec_ind();
        m_out->println("end");
        m_out->println("$cast(api, exec_b.get_api());");
        m_out->println("begin");
        m_out->inc_ind();
    }
    for (std::vector<arl::dm::ITypeExecUP>::const_iterator
        it=t.begin();
        it!=t.end(); it++) {
        arl::dm::ITypeExecProc *tp = dynamic_cast<arl::dm::ITypeExecProc *>(it->get());
        TaskGenerateExecScope(m_gen, m_genref, m_out).generate(tp->getBody(), it!=t.begin());
    }
    if (executor) {
        m_out->dec_ind();
        m_out->println("end");
    }

    m_out->dec_ind();
    m_out->println("end%s", (istask)?"task":"function");
    m_out->println("");
    DEBUG_LEAVE("generate %s", fname.c_str());
}

}
}
}
}
