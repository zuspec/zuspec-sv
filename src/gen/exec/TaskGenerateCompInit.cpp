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
    IGenRefExpr         *genref,
    IOutput             *out) : TaskGenerateStructInit(gen, genref, out) {
    m_dbg = 0;
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateCompInit", gen->getDebugMgr());
}

TaskGenerateCompInit::~TaskGenerateCompInit() {

}

void TaskGenerateCompInit::visitDataTypeComponent(arl::dm::IDataTypeComponent *t) {
    DEBUG_ENTER("visitDataTypeComponent");
    m_out->println("%s.init(exec_b);", m_field->name().c_str());
    DEBUG_LEAVE("visitDataTypeComponent");
}

}
}
}
}
