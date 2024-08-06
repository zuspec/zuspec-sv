/*
 * TaskGenerateAction.cpp
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
#include "GenRefExprExecModel.h"
#include "TaskGenerate.h"
#include "TaskGenerateAction.h"
#include "TaskGenerateActionConstraints.h"
#include "TaskGenerateActionFields.h"
#include "TaskGenerateExecBlock.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateAction::TaskGenerateAction(
    TaskGenerate        *gen,
    IOutput             *out) : TaskGenerateStruct(gen, out) {
    m_dbg = 0;
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateAction", gen->getDebugMgr());

}

TaskGenerateAction::~TaskGenerateAction() {

}

void TaskGenerateAction::generate_head(vsc::dm::IDataTypeStruct *t) {
    m_out->println("class %s extends action;", m_gen->getNameMap()->getName(t).c_str());
    m_out->inc_ind();
}

void TaskGenerateAction::generate_fields(vsc::dm::IDataTypeStruct *t) {
    TaskGenerateActionFields(m_gen, m_out).generate(t);
}

void TaskGenerateAction::generate_constraints(vsc::dm::IDataTypeStruct *t) {
    GenRefExprExecModel genref(
        m_gen,
        t,
        "this",
        false);
    TaskGenerateActionConstraints(m_gen, &genref, m_out).generate(t);
}

void TaskGenerateAction::generate_execs(vsc::dm::IDataTypeStruct *t) {
    TaskGenerateStruct::generate_execs(t);

    GenRefExprExecModel genref(
        m_gen,
        t,
        "this",
        false);

    std::vector<std::pair<arl::dm::ExecKindT,std::pair<bool,std::string>>> exec_t = {
        {arl::dm::ExecKindT::Body, {true, "body"}}
    };

    for (auto it=exec_t.begin(); it!=exec_t.end(); it++) {
        const std::vector<arl::dm::ITypeExecUP> &execs = 
            dynamic_cast<arl::dm::IDataTypeArlStruct *>(t)->getExecs(it->first);
        
        if (execs.size()) {
            TaskGenerateExecBlock(m_gen, &genref, m_out).generate(
                execs, it->second.first, it->second.second);
        }
    }
}

void TaskGenerateAction::visitTypeFieldRef(vsc::dm::ITypeFieldRef *f) {
    if (f->name() != "comp") {
        TaskGenerateStruct::visitTypeFieldRef(f);
    }
}

}
}
}
}
