/*
 * TaskGenerateStructCtor.cpp
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
#include "TaskGenerateStructCtor.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateStructCtor::TaskGenerateStructCtor(
    TaskGenerate        *gen,
    IOutput             *out) : m_dbg(0), m_gen(gen), m_out(out) {
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateStructCtor", gen->getDebugMgr());
}

TaskGenerateStructCtor::~TaskGenerateStructCtor() {

}

void TaskGenerateStructCtor::generate_head(vsc::dm::IDataTypeStruct *t) {
    m_out->println("function new();");
    m_out->inc_ind();
}

void TaskGenerateStructCtor::generate(vsc::dm::IDataTypeStruct *t) {
    generate_head(t);
    for (std::vector<vsc::dm::ITypeFieldUP>::const_iterator
        it=t->getFields().begin();
        it!=t->getFields().end(); it++) {
        (*it)->accept(m_this);
    }
    generate_tail(t);
}

void TaskGenerateStructCtor::generate_tail(vsc::dm::IDataTypeStruct *t) {
    m_out->dec_ind();
    m_out->println("endfunction");
}

void TaskGenerateStructCtor::visitTypeField(vsc::dm::ITypeField *f) {
    m_field = f;
    f->getDataType()->accept(m_this);
}

}
}
}
}
