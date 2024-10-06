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
#include "vsc/dm/impl/TaskIsDataTypeStruct.h"
#include "GenRefExprExecModel.h"
#include "TaskGenerate.h"
#include "TaskGenerateDataType.h"
#include "TaskGenerateExpr.h"
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
    DEBUG_ENTER("generate");
    GenRefExprExecModel genref(
        m_gen,
        t,
        "this",
        false);
    m_genref = &genref;

    for (std::vector<vsc::dm::ITypeFieldUP>::const_iterator
        it=t->getFields().begin();
        it!=t->getFields().end(); it++) {
        (*it)->accept(m_this);
    }
    DEBUG_LEAVE("generate");
}

void TaskGenerateStructFields::visitDataTypeAddrHandle(arl::dm::IDataTypeAddrHandle *t) {
    DEBUG_ENTER("visitDataTypeAddrHandle");
    m_out->println("addr_handle_t %s;", m_field->name().c_str());
    DEBUG_LEAVE("visitDataTypeAddrHandle");
}

void TaskGenerateStructFields::visitDataTypeArray(vsc::dm::IDataTypeArray *t) {
    DEBUG_ENTER("visitDataTypeArray");
    m_out->indent();
    m_out->write("%sarray_c #(",
        qualifiers(m_field->getAttr()).c_str());
    TaskGenerateDataType(m_gen, m_out).generate(t->getElemType());
    m_out->write(", %d) %s;\n", 
        t->getSize(),
        m_field->name().c_str());
    DEBUG_LEAVE("visitDataTypeArray");
}

void TaskGenerateStructFields::visitDataTypeBool(vsc::dm::IDataTypeBool *t) {
    DEBUG_ENTER("visitDataTypeBool");
    m_out->println("%sbit %s;", 
        qualifiers(m_field->getAttr()).c_str(),
        m_field->name().c_str());
    DEBUG_LEAVE("visitDataTypeBool");
}

void TaskGenerateStructFields::visitDataTypeInt(vsc::dm::IDataTypeInt *t) {
    DEBUG_ENTER("visitDataTypeInt");
    m_out->indent();
    m_out->write("%sbit%s[%d:0] %s", 
        qualifiers(m_field->getAttr()).c_str(),
        (t->isSigned())?" signed":"",
        (t->getWidth()-1),
        m_field->name().c_str());
    
    if (m_init) {
        m_out->write(" = ");
        TaskGenerateExpr(m_gen, m_genref, m_out).generate(m_init);
    }

    m_out->write(";\n");
    DEBUG_LEAVE("visitDataTypeInt");
}

void TaskGenerateStructFields::visitDataTypeList(vsc::dm::IDataTypeList *t) {
    DEBUG_ENTER("visitDataTypeList");
    m_out->indent();
    m_out->write("%slist_c #(",
        qualifiers(m_field->getAttr()).c_str());
    TaskGenerateDataType(m_gen, m_out).generate(t->getElemType());
    m_out->write(") %s;\n", m_field->name().c_str());
    DEBUG_LEAVE("visitDataTypeList");
}

void TaskGenerateStructFields::visitDataTypeString(vsc::dm::IDataTypeString *t) {

}

void TaskGenerateStructFields::visitDataTypeComponent(arl::dm::IDataTypeComponent*t) {
    DEBUG_ENTER("visitDataTypeComponent");
    std::string qual;

    if ((m_field->getAttr() & vsc::dm::TypeFieldAttr::Rand) != vsc::dm::TypeFieldAttr::NoAttr) {
        qual = "rand ";
    }

    m_out->println("%s%s %s;", qual.c_str(),
        m_gen->getNameMap()->getName(t).c_str(),
        m_field->name().c_str());
    DEBUG_LEAVE("visitDataTypeComponent");
}

void TaskGenerateStructFields::visitDataTypeStruct(vsc::dm::IDataTypeStruct *t) {
    DEBUG_ENTER("visitDataTypeStruct");
    // TODO: 'new' struct?
    m_out->println("%s%s %s;", 
        qualifiers(m_field->getAttr()).c_str(),
        m_gen->getNameMap()->getName(t).c_str(),
        m_field->name().c_str());
    DEBUG_LEAVE("visitDataTypeStruct");
}

void TaskGenerateStructFields::visitTypeField(vsc::dm::ITypeField *f) {
    DEBUG_ENTER("visitTypeField %s", f->name().c_str());
    m_field = f;
    f->getDataType()->accept(m_this);
    DEBUG_LEAVE("visitTypeField %s", f->name().c_str());
}

void TaskGenerateStructFields::visitTypeFieldPhy(vsc::dm::ITypeFieldPhy *f) {
    DEBUG_ENTER("visitTypeFieldPhy (init=%p)", f->getInit());
    m_field = f;
    m_init = f->getInit();
    f->getDataType()->accept(m_this);
    DEBUG_LEAVE("visitTypeFieldPhy");
}

void TaskGenerateStructFields::visitTypeFieldRef(vsc::dm::ITypeFieldRef *f) {
    DEBUG_ENTER("visitTypeFieldRef");
    m_field = f;
    m_init = 0;
    f->getDataType()->accept(m_this);
    DEBUG_LEAVE("visitTypeFieldRef");
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
