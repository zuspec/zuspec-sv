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
#include "ActivityInfo.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateInlineConstraints::TaskGenerateInlineConstraints(
        TaskGenerateActorPkgPrv *gen,
        IGenRefExpr             *genref,
        IOutput                 *out
    ) : m_gen(gen), m_genref(genref), m_out(out) {
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
        DEBUG("Have an inline constraint");
    }
    DEBUG_LEAVE("visitDataTypeActivityTraverseType");
}

dmgr::IDebug *TaskGenerateInlineConstraints::m_dbg = 0;

}
}
}
}
