/*
 * TaskGenerateExprVal.cpp
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
#include "vsc/dm/impl/ValRefArr.h"
#include "vsc/dm/impl/ValRefBool.h"
#include "vsc/dm/impl/ValRefInt.h"
#include "vsc/dm/impl/ValRefStr.h"
#include "vsc/dm/impl/ValRefStruct.h"
#include "TaskGenerate.h"
#include "TaskGenerateExprVal.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateExprVal::TaskGenerateExprVal(
        TaskGenerate        *gen,
        IOutput             *out) : m_dbg(0), m_gen(gen), m_out(out) {
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateExprVal", gen->getDebugMgr());
}

TaskGenerateExprVal::~TaskGenerateExprVal() {

}

void TaskGenerateExprVal::generate(vsc::dm::ITypeExprVal *e) {
    m_val = e->val();
    e->type()->accept(m_this);
};

void TaskGenerateExprVal::visitDataTypeArray(vsc::dm::IDataTypeArray *t) { 

}

void TaskGenerateExprVal::visitDataTypeBool(vsc::dm::IDataTypeBool *t) { 
    vsc::dm::ValRefBool val(m_val);
    m_out->write("%s", val.get_val()?"true":"false");
}

void TaskGenerateExprVal::visitDataTypeEnum(vsc::dm::IDataTypeEnum *t) { 

}

void TaskGenerateExprVal::visitDataTypeInt(vsc::dm::IDataTypeInt *t) { 
    vsc::dm::ValRefInt val(m_val);
    if (t->isSigned()) {
        m_out->write("%d'd%lld", t->width(), val.get_val_s());
    } else {
        m_out->write("%d'h%llx", t->width(), val.get_val_u());
    }
}

void TaskGenerateExprVal::visitDataTypePtr(vsc::dm::IDataTypePtr *t) { }

void TaskGenerateExprVal::visitDataTypeString(vsc::dm::IDataTypeString *t) { }

void TaskGenerateExprVal::visitDataTypeStruct(vsc::dm::IDataTypeStruct *t) { }

}
}
}
}
