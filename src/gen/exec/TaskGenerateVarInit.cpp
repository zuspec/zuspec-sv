/*
 * TaskGenerateVarInit.cpp
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
#include "TaskGenerateExpr.h"
#include "TaskGenerateVarInit.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateVarInit::TaskGenerateVarInit(
    TaskGenerate            *gen,
    IGenRefExpr             *genref,
    IOutput                 *out) :
        m_gen(gen), m_genref(genref), m_out(out) {
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateVarInit", gen->getDebugMgr());
}

TaskGenerateVarInit::~TaskGenerateVarInit() {

}

void TaskGenerateVarInit::generate(arl::dm::ITypeProcStmtVarDecl *var) {
    DEBUG_ENTER("generate");
    m_var = var;
    var->getDataType()->accept(m_this);
    DEBUG_LEAVE("generate");
}

void TaskGenerateVarInit::visitDataTypeBool(vsc::dm::IDataTypeBool *t) { 
    DEBUG_ENTER("visitDataTypeBool");
    if (m_var->getInit()) {
        m_out->indent();
        m_out->write("%s = ");
        TaskGenerateExpr(m_gen, m_genref, m_out).generate(m_var->getInit());
        m_out->write(";\n");
    }
    DEBUG_LEAVE("visitDataTypeBool");
}

void TaskGenerateVarInit::visitDataTypeInt(vsc::dm::IDataTypeInt *t) {
    DEBUG_ENTER("visitDataTypeInt");
    if (m_var->getInit()) {
        m_out->indent();
        m_out->write("%s = ");
        TaskGenerateExpr(m_gen, m_genref, m_out).generate(m_var->getInit());
        m_out->write(";\n");
    }
    DEBUG_LEAVE("visitDataTypeInt");
}

void TaskGenerateVarInit::visitDataTypeString(vsc::dm::IDataTypeString *t) { 
    DEBUG_ENTER("visitDataTypeString");
    if (m_var->getInit()) {
        m_out->indent();
        m_out->write("%s = ");
        TaskGenerateExpr(m_gen, m_genref, m_out).generate(m_var->getInit());
        m_out->write(";\n");
    }
    DEBUG_LEAVE("visitDataTypeString");

}

void TaskGenerateVarInit::visitDataTypeStruct(vsc::dm::IDataTypeStruct *t) { 
    DEBUG_ENTER("visitDataTypeStruct");
    if (m_var->getInit()) {

    } else {
        m_out->println("%s = %s::create_default();",
            m_var->name().c_str(),
            m_gen->getNameMap()->getName(t).c_str());
    }
    DEBUG_LEAVE("visitDataTypeStruct");
}

dmgr::IDebug *TaskGenerateVarInit::m_dbg = 0;

}
}
}
}
