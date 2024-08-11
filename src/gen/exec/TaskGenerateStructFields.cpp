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

void TaskGenerateStructFields::visitDataTypeBool(vsc::dm::IDataTypeBool *t) {
    m_out->println("%sbit %s;", 
        qualifiers(m_field->getAttr()).c_str(),
        m_field->name().c_str());
}

void TaskGenerateStructFields::visitDataTypeInt(vsc::dm::IDataTypeInt *t) {
    m_out->println("%sbit%s[%d:0] %s;", 
        qualifiers(m_field->getAttr()).c_str(),
        (t->isSigned())?" signed":"",
        (t->getWidth()-1),
        m_field->name().c_str());
}

void TaskGenerateStructFields::visitDataTypeString(vsc::dm::IDataTypeString *t) {

}

void TaskGenerateStructFields::visitDataTypeComponent(arl::dm::IDataTypeComponent*t) {
    std::string qual;

    if ((m_field->getAttr() & vsc::dm::TypeFieldAttr::Rand) != vsc::dm::TypeFieldAttr::NoAttr) {
        qual = "rand ";
    }

    m_out->println("%s%s %s;", qual.c_str(),
        m_gen->getNameMap()->getName(t).c_str(),
        m_field->name().c_str());
}

void TaskGenerateStructFields::visitDataTypeStruct(vsc::dm::IDataTypeStruct *t) {
    m_out->println("%s%s %s;", 
        qualifiers(m_field->getAttr()).c_str(),
        m_gen->getNameMap()->getName(t).c_str(),
        m_field->name().c_str());
}

void TaskGenerateStructFields::visitTypeField(vsc::dm::ITypeField *f) {
    m_field = f;
    f->getDataType()->accept(m_this);
}

std::string TaskGenerateStructFields::qualifiers(vsc::dm::TypeFieldAttr attr) {
    std::string ret;

    DEBUG("qualifiers: 0x%08x", attr);

    if ((attr & vsc::dm::TypeFieldAttr::Rand) != vsc::dm::TypeFieldAttr::NoAttr) {
        ret += "rand ";
    }

    return ret;
}

}
}
}
}
