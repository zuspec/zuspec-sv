/*
 * TaskDefineType.cpp
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
#include "ICustomGen.h"
#include "GenRefExprExecModel.h"
#include "TaskGenerate.h"
#include "TaskGenerateAction.h"
#ifdef UNDEFINED
#include "TaskActivity.h"
#include "TaskAddrClaim.h"
#endif
#include "TaskGenerateAddrSpace.h"
#include "TaskGenerateComp.h"
#include "TaskGenerateFunction.h"
#include "TaskDefineType.h"
#ifdef UNDEFINED
#include "TaskPackedStruct.h"
#endif
#include "TaskGenerateRegGroup.h"
#include "TaskGenerateStruct.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskDefineType::TaskDefineType(
    TaskGenerate       *gen,
    IOutput            *out) : m_gen(gen), m_genref(0), m_out(out) {
    DEBUG_INIT("zsp::be::sw::TaskDefineType", gen->getDebugMgr());
}

TaskDefineType::TaskDefineType(
    TaskGenerate       *gen,
    IGenRefExpr        *genref,
    IOutput            *out) : m_gen(gen), m_genref(genref), m_out(out) {
    DEBUG_INIT("zsp::be::sw::TaskDefineType", gen->getDebugMgr());
}

TaskDefineType::~TaskDefineType() {

}

void TaskDefineType::generate(vsc::dm::IDataType *item) {
    ICustomGen *custom_gen = 
        dynamic_cast<ICustomGen *>(item->getAssociatedData());
    if (custom_gen) {
//        custom_gen->genDefinition(m_gen, m_out_h, m_out_c, item);
    } else {
        item->accept(m_this);
    }
}

void TaskDefineType::generate(arl::dm::IDataTypeFunction *item) {
    ICustomGen *custom_gen = 
        dynamic_cast<ICustomGen *>(item->getAssociatedData());
    if (custom_gen) {
        custom_gen->genFunctionDefinition(m_gen, m_out, m_genref, item);
    } else {
        item->accept(m_this);
    }
}

void TaskDefineType::generate_dflt(vsc::dm::IDataType *item) {
    item->accept(m_this);
}

void TaskDefineType::visitDataTypeAction(arl::dm::IDataTypeAction *i) { 
    DEBUG_ENTER("visitDataTypeAction %s", i->name().c_str());
    TaskGenerateAction(m_gen, m_out).generate(i);
    DEBUG_LEAVE("visitDataTypeAction");
}

void TaskDefineType::visitDataTypeActivity(arl::dm::IDataTypeActivity *t) { 
    DEBUG_ENTER("visitDataTypeActivity");
//    TaskActivity(m_gen).generate(t);
    DEBUG_LEAVE("visitDataTypeActivity");
}

void TaskDefineType::visitDataTypeAddrClaim(arl::dm::IDataTypeAddrClaim *t) {
    DEBUG_ENTER("visitDataTypeAddrClaim");
//    TaskAddrClaim(m_gen, m_out_h, m_out_c).generate(t);
    DEBUG_LEAVE("visitDataTypeAddrClaim");
}

void TaskDefineType::visitDataTypeAddrSpaceC(arl::dm::IDataTypeAddrSpaceC *t) {
    DEBUG_ENTER("visitDataTypeAddrSpaceC");
    TaskGenerateAddrSpace(m_gen, m_out).generate(t);
    DEBUG_LEAVE("visitDataTypeAddrSpaceC");
}

void TaskDefineType::visitDataTypeAddrSpaceTransparentC(arl::dm::IDataTypeAddrSpaceTransparentC *t) {
    DEBUG_ENTER("visitDataTypeAddrSpaceTransparentC");
    TaskGenerateAddrSpace(m_gen, m_out).generate(t);
    DEBUG_LEAVE("visitDataTypeAddrSpaceTransparentC");
}

void TaskDefineType::visitDataTypeComponent(arl::dm::IDataTypeComponent *t) { 
    DEBUG_ENTER("visitDataTypeComponent");
    TaskGenerateComp(m_gen, m_out).generate(t);
    DEBUG_LEAVE("visitDataTypeComponent");
}

void TaskDefineType::visitDataTypeFunction(arl::dm::IDataTypeFunction *t) { 
    if (t->getImportSpecs().size() == 0) {
        if (m_genref) {
            TaskGenerateFunction(m_gen, m_genref, m_out).generate(t, false);
        } else {
            GenRefExprExecModel genref(m_gen, 0, "", false);
            TaskGenerateFunction(m_gen, &genref, m_out).generate(t, false);
        }
    }
}

void TaskDefineType::visitDataTypePackedStruct(arl::dm::IDataTypePackedStruct *t) {
    DEBUG_ENTER("visitDataTypePackedStruct");
//    TaskPackedStruct(m_gen, m_out_h, m_out_c).generate(t);
    DEBUG_LEAVE("visitDataTypePackedStruct");
}

void TaskDefineType::visitDataTypeRegGroup(arl::dm::IDataTypeRegGroup *t) {
    DEBUG_ENTER("visitDataTypeRegGroup");
    TaskGenerateRegGroup(m_gen, m_out).generate(t);
    DEBUG_LEAVE("visitDataTypeRegGroup");
}

void TaskDefineType::visitDataTypeStruct(vsc::dm::IDataTypeStruct *t) {
    DEBUG_ENTER("visitDataTypeStruct %s", t->name().c_str());
    TaskGenerateStruct(m_gen, m_out).generate(t);
    DEBUG_LEAVE("visitDataTypeStruct");
} 

dmgr::IDebug *TaskDefineType::m_dbg = 0;

}
}
}
}
