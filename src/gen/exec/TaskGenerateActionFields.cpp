/*
 * TaskGenerateActionFields.cpp
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
#include "TaskGenerateActionFields.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateActionFields::TaskGenerateActionFields(
        TaskGenerate        *gen,
        IOutput             *out) : TaskGenerateStructFields(gen, out) {
    m_dbg = 0;
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateActionFields", gen->getDebugMgr());
}

TaskGenerateActionFields::~TaskGenerateActionFields() {

}

void TaskGenerateActionFields::visitTypeFieldRef(vsc::dm::ITypeFieldRef *f) {
    DEBUG_ENTER("visitTypeFieldRef %s", f->name().c_str());
    if (f->name() != "comp") {
        TaskGenerateStructFields::visitTypeFieldRef(f);
    } else {
        m_out->println("%s comp;", 
            m_gen->getNameMap()->getName(f->getDataType()).c_str());
    }
    DEBUG_LEAVE("visitTypeFieldRef %s", f->name().c_str());
}

}
}
}
}
