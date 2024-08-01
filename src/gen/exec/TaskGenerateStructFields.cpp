/*
 * TaskGenerateStructFields.cpp
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
#include "TaskGenerateStructFields.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateStructFields::TaskGenerateStructFields(
    TaskGenerate        *gen,
    IOutput             *out) : m_dbg(0), m_gen(gen), m_out(out), m_field(0) {
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateStructFields", gen->getDebugMgr());
}

TaskGenerateStructFields::~TaskGenerateStructFields() {

}

void TaskGenerateStructFields::generate(vsc::dm::IDataTypeStruct *t) {
    for (std::vector<vsc::dm::ITypeFieldUP>::const_iterator
        it=t->getFields().begin();
        it!=t->getFields().end(); it++) {
        (*it)->accept(m_this);
    }
}

void TaskGenerateStructFields::visitDataTypeComponent(arl::dm::IDataTypeComponent*t) {
    m_out->println("%s %s;", 
        m_gen->getNameMap()->getName(t).c_str(),
        m_field->name().c_str());
}

void TaskGenerateStructFields::visitTypeField(vsc::dm::ITypeField *f) {
    m_field = f;
    f->getDataType()->accept(m_this);
}

}
}
}
}
