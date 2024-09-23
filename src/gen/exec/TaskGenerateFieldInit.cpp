/*
 * TaskGenerateFieldInit.cpp
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
#include "TaskGenerateFieldInit.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateFieldInit::TaskGenerateFieldInit(
    TaskGenerate            *gen,
    IGenRefExpr             *genref,
    IOutput                 *out) :
        m_gen(gen), m_genref(genref), m_out(out) {
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateFieldInit", gen->getDebugMgr());
}

TaskGenerateFieldInit::~TaskGenerateFieldInit() {

}

void TaskGenerateFieldInit::generate(vsc::dm::ITypeField *field) {
    DEBUG_ENTER("generate");
    field->accept(m_this);
    DEBUG_LEAVE("generate");
}

void TaskGenerateFieldInit::visitDataTypeInt(vsc::dm::IDataTypeInt *t) {

}

void TaskGenerateFieldInit::visitTypeFieldPhy(vsc::dm::ITypeFieldPhy *f) {
    DEBUG_ENTER("visitTypeFieldPhy");
    if (f->getInit()) {
        TaskGenerateExpr(m_gen, m_genref, m_out).generate(f->getInit());
    } else {
        f->getDataType()->accept(m_this);
    }
    DEBUG_LEAVE("visitTypeFieldPhy");
}

dmgr::IDebug *TaskGenerateFieldInit::m_dbg = 0;

}
}
}
}
