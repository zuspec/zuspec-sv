/*
 * TaskPrepContext.cpp
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
#include "TaskPrepContext.h"
#include "TaskRewriteTargetRvCalls.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskPrepContext::TaskPrepContext(
    dmgr::IDebugMgr             *dmgr,
    arl::dm::IContext           *ctxt,
    arl::eval::IFactory         *eval_f) :
    m_dmgr(dmgr), m_ctxt(ctxt), m_eval_f(eval_f) {
    DEBUG_INIT("zsp::sv::gen::exec::TaskPrepContext", dmgr);
}

TaskPrepContext::~TaskPrepContext() {

}

void TaskPrepContext::prepare() {
    DEBUG_ENTER("prepare");

    for (std::vector<vsc::dm::IDataTypeStructUP>::const_iterator
        it=m_ctxt->getDataTypeStructs().begin();
        it!=m_ctxt->getDataTypeStructs().end(); it++) {
        (*it)->accept(m_this);
    }

    DEBUG_LEAVE("prepare");
}

void TaskPrepContext::visitDataTypeAction(arl::dm::IDataTypeAction *t) {
    DEBUG_ENTER("visitDataTypeAction %s", t->name().c_str());
    const std::vector<arl::dm::ITypeExecUP> &execs = t->getExecs(arl::dm::ExecKindT::Body);

    for (std::vector<arl::dm::ITypeExecUP>::const_iterator
        it=execs.begin();
        it!=execs.end(); it++) {
        (*it)->accept(m_this);
    }

    DEBUG_LEAVE("visitDataTypeAction %s", t->name().c_str());
}

void TaskPrepContext::visitTypeExecProc(arl::dm::ITypeExecProc *e) {
    DEBUG_ENTER("visitTypeExecProc");
    TaskRewriteTargetRvCalls(m_dmgr, m_ctxt).rewrite(e->getBody());
    DEBUG_LEAVE("visitTypeExecProc");
}

dmgr::IDebug *TaskPrepContext::m_dbg = 0;

}
}
}
}