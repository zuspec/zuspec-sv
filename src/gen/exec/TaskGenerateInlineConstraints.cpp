/*
 * TaskGenerateInlineConstraints.cpp
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
#include "TaskGenerateInlineConstraints.h"
#include "dmgr/impl/DebugMacros.h"
#include "TaskGenerate.h"
#include "TaskGenerateConstraint.h"
#include "TaskGenerateActorPkgPrv.h"
#include "TaskGenerateInlineConstraint.h"
#include "ActivityInfo.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateInlineConstraints::TaskGenerateInlineConstraints(
        TaskGenerateActorPkgPrv     *gen,
        IGenRefExpr                 *genref,
        vsc::dm::IDataTypeStruct    *action_t,
        IOutput                     *out) : 
        m_gen(gen), m_genref(genref), m_action_t(action_t), m_out(out), m_id(1) {
    DEBUG_INIT("Zsp::sv::gen::exec::TaskGenerateInlineConstraints", gen->getDebugMgr());    
}

TaskGenerateInlineConstraints::~TaskGenerateInlineConstraints() {

}

void TaskGenerateInlineConstraints::generate(arl::dm::IDataTypeActivity *activity) {
    DEBUG_ENTER("generate");
    activity->accept(this);
    DEBUG_LEAVE("generate");
}

void TaskGenerateInlineConstraints::visitDataTypeActivityTraverseType(arl::dm::IDataTypeActivityTraverseType *t) {
    DEBUG_ENTER("visitDataTypeActivityTraverseType");
    if (t->getWithC()) {
        // Assign a name to this inline constraint
        char tmp[64];
        DEBUG("Have an inline constraint");
        sprintf(tmp, "_%d", m_id++);
        std::string constraint_name = m_gen->getNameMap()->getName(m_action_t) + tmp;
        m_gen->getNameMap()->setName(t->getWithC(), constraint_name);
        TaskGenerateInlineConstraint(
            m_gen, 
            m_genref,
            m_out).generate(
                t->getWithC(),
                constraint_name,
                t->getTarget(),
                m_action_t);
    }
    DEBUG_LEAVE("visitDataTypeActivityTraverseType");
}

dmgr::IDebug *TaskGenerateInlineConstraints::m_dbg = 0;

}
}
}
}
