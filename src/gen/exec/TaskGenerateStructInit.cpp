/*
 * TaskGenerateStructInit.cpp
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
#include "TaskGenerate.h"
#include "TaskGenerateStructInit.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateStructInit::TaskGenerateStructInit(
    TaskGenerate        *gen,
    IGenRefExpr         *genref,
    IOutput             *out) : 
        m_dbg(0), m_gen(gen), m_genref(genref), m_out(out) {

}

TaskGenerateStructInit::~TaskGenerateStructInit() {

}


void TaskGenerateStructInit::generate_head(vsc::dm::IDataTypeStruct *t) {
    m_out->println("virtual function void init(executor_base exec_b);");
}

void TaskGenerateStructInit::generate(vsc::dm::IDataTypeStruct *t) {
    generate_head(t);
    for (std::vector<vsc::dm::ITypeFieldUP>::const_iterator
        it=t->getFields().begin();
        it!=t->getFields().end(); it++) {
        (*it)->accept(m_this);
    }
    generate_tail(t);
}

void TaskGenerateStructInit::generate_tail(vsc::dm::IDataTypeStruct *t) {
    m_out->println("endfunction");
}

void TaskGenerateStructInit::visitDataTypeArray(vsc::dm::IDataTypeArray *t) {
    DEBUG_ENTER("visitDataTypeArray");
    if (m_field->getInit()) {
        for (uint32_t i=0; i<t->getSize(); i++) {

        }
    }
    DEBUG_LEAVE("visitDataTypeArray");
}

void TaskGenerateStructInit::visitDataTypeComponent(arl::dm::IDataTypeComponent *t) {
    m_out->println("%s = new(this, \"%s\");",
        m_field->name().c_str(),
        m_field->name().c_str());
}

void TaskGenerateStructInit::visitDataTypeStruct(vsc::dm::IDataTypeStruct *t) {
    if (m_field->getInit()) {
        m_out->println("%s = %s::create_init();",
            m_field->name().c_str(),
            m_gen->getNameMap()->getName(t).c_str());
    } else {
        m_out->println("%s = %s::create_default();",
            m_field->name().c_str(),
            m_gen->getNameMap()->getName(t).c_str());
    }
}

void TaskGenerateStructInit::visitTypeFieldPhy(vsc::dm::ITypeFieldPhy *f) {
    m_field = f;
    f->getDataType()->accept(m_this);
}

void TaskGenerateStructInit::visitTypeFieldRef(vsc::dm::ITypeFieldRef *f) {

}

}
}
}
}
