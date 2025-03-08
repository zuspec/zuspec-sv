/*
 * TaskGenerateComp.cpp
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
#include "TaskGenerateComp.h"
#include "TaskGenerateCompCheck.h"
#include "TaskGenerateCompCtor.h"
#include "TaskGenerateCompDoInit.h"
#include "TaskGenerateCompInit.h"
#include "TaskGenerateCompFields.h"
#include "TaskGenerateExecBlock.h"
#include "TaskGenerateStructInit.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateComp::TaskGenerateComp(
    TaskGenerate        *gen,
    IOutput             *out) : TaskGenerateStruct(gen, out) {
    m_dbg = 0;
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateComp", gen->getDebugMgr());

}

TaskGenerateComp::~TaskGenerateComp() {

}

void TaskGenerateComp::generate_head(vsc::dm::IDataTypeStruct *t) {
    std::string super = "component_c";
    if (t->getSuper()) {
        super = m_gen->getNameMap()->getName(t->getSuper());
    }
    m_out->println("class %s extends %s;", 
        m_gen->getNameMap()->getName(t).c_str(),
        super.c_str());
    m_out->inc_ind();
    m_out->println("`zsp_component_util(%s)",
        m_gen->getNameMap()->getName(t).c_str());
}

void TaskGenerateComp::generate(vsc::dm::IDataTypeStruct *t) {
    TaskGenerateStruct::generate(t);
}

void TaskGenerateComp::generate_check(vsc::dm::IDataTypeStruct *t) {
    TaskGenerateCompCheck(m_gen, m_out).generate(t);
}

void TaskGenerateComp::generate_ctor(vsc::dm::IDataTypeStruct *t) {
    TaskGenerateCompCtor(m_gen, m_out).generate(t);
    m_out->println("");
}

void TaskGenerateComp::generate_init(vsc::dm::IDataTypeStruct *t) {
    DEBUG_ENTER("generate_init");
    DEBUG("Skip");
    // Generate the 'init' at the same time
    // Note: class library currently handles this
//    TaskGenerateCompDoInit(m_gen, m_out).generate(t);
//    m_out->println("");
    /*
    TaskGenerateCompInit(m_gen, 0, m_out).generate(t);
    m_out->println("");
     */
    DEBUG_LEAVE("generate_init");
}

void TaskGenerateComp::generate_fields(vsc::dm::IDataTypeStruct *t) {
    TaskGenerateCompFields(m_gen, m_out).generate(t);
}

void TaskGenerateComp::generate_execs(vsc::dm::IDataTypeStruct *t) {
    GenRefExprExecModel genref(
        m_gen,
        t,
        "this",
        false);

    std::vector<std::pair<arl::dm::ExecKindT,std::pair<bool,std::string>>> exec_t = {
        {arl::dm::ExecKindT::InitDown, {false, "init_down"}},
        {arl::dm::ExecKindT::InitUp, {false, "init_up"}},
    };

    for (auto it=exec_t.begin(); it!=exec_t.end(); it++) {
        const std::vector<arl::dm::ITypeExecUP> &execs = 
            dynamic_cast<arl::dm::IDataTypeArlStruct *>(t)->getExecs(it->first);
        
        if (execs.size()) {
            TaskGenerateExecBlock(m_gen, &genref, m_out).generate(
                execs, it->second.first, true, it->second.second);
        }
    }
    generate_check(t);
}

}
}
}
}
