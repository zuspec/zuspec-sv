/*
 * TaskGenerateCompFields.cpp
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
#include "TaskGenerateCompFields.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateCompFields::TaskGenerateCompFields(
    TaskGenerate            *gen,
    IOutput                 *out) : TaskGenerateStructFields(gen, out) {
    m_dbg = 0;
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateCompFields", gen->getDebugMgr());

}

TaskGenerateCompFields::~TaskGenerateCompFields() {

}

void TaskGenerateCompFields::visitTypeFieldRegGroup(arl::dm::ITypeFieldRegGroup *f) {
    DEBUG_ENTER("visitTypeFieldRegGroup");
    m_out->println("reg_group_field_c #(%s) %s;", 
        m_gen->getNameMap()->getName(f->getDataType()).c_str(),
        f->name().c_str());
    DEBUG_LEAVE("visitTypeFieldRegGroup");
}

}
}
}
}
