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
#include <tuple>
#include "dmgr/impl/DebugMacros.h"
#include "GenRefExprExecModel.h"
#include "TaskGenerate.h"
#include "TaskGenerateAction.h"
#include "TaskGenerateActionConstraints.h"
#include "TaskGenerateActionCtor.h"
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
    arl::dm::IDataTypeAction *action_t = dynamic_cast<arl::dm::IDataTypeAction *>(t);
    m_out->println("typedef class %s;", 
        m_gen->getNameMap()->getName(action_t->getComponentType()).c_str());
    m_out->println("class %s extends action #(executor_base_c);", m_gen->getNameMap()->getName(t).c_str());
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

void TaskGenerateAction::generate_ctor(vsc::dm::IDataTypeStruct *t) {
    TaskGenerateActionCtor(m_gen, m_out).generate(t);
}

void TaskGenerateAction::generate_execs(vsc::dm::IDataTypeStruct *t) {
    using std::get;
    TaskGenerateStruct::generate_execs(t);

    GenRefExprExecModel genref(
        m_gen,
        t,
        "this",
        false);

    std::vector<std::tuple<arl::dm::ExecKindT,bool,bool,std::string>> exec_t = {
        {arl::dm::ExecKindT::Body, true, true, "body"}
    };

    for (auto it=exec_t.begin(); it!=exec_t.end(); it++) {
        const std::vector<arl::dm::ITypeExecUP> &execs = 
            dynamic_cast<arl::dm::IDataTypeArlStruct *>(t)->getExecs(get<0>(*it));
        
        if (execs.size()) {
            TaskGenerateExecBlock(m_gen, &genref, m_out).generate(
                execs, 
                get<1>(*it),  // task
                get<2>(*it),  // executor,
                get<3>(*it)); // name
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
