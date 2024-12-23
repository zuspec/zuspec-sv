/*
 * TaskGenerateStruct.cpp
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
#include "TaskGenerateExecBlock.h"
#include "TaskGenerateStruct.h"
#include "TaskGenerateStructAssign.h"
#include "TaskGenerateStructConstraints.h"
#include "TaskGenerateStructCreateInit.h"
#include "TaskGenerateStructCtor.h"
#include "TaskGenerateStructDtor.h"
#include "TaskGenerateStructDoSolveExec.h"
#include "TaskGenerateStructFields.h"
#include "TaskGenerateStructInit.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateStruct::TaskGenerateStruct(
    TaskGenerate            *gen,
    IOutput                 *out) : m_dbg(0), m_gen(gen), m_out(out) {
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateStruct", gen->getDebugMgr());

}

TaskGenerateStruct::~TaskGenerateStruct() {

}

void TaskGenerateStruct::generate_head(vsc::dm::IDataTypeStruct *t) {
    m_out->println("class %s extends object;", 
        m_gen->getNameMap()->getName(t).c_str());
    m_out->inc_ind();
}

void TaskGenerateStruct::generate(vsc::dm::IDataTypeStruct *t) {
    generate_head(t);
    generate_fields(t);
    m_out->println("");
    generate_ctor(t);
    m_out->println("");
    generate_dtor(t);
    m_out->println("");
    generate_init(t);
    generate_create_assign(t);
    m_out->println("");
    generate_create_default(t);
    m_out->println("");
    generate_create_init(t);

    generate_constraints(t);
    generate_execs(t);
    generate_methods(t);
    generate_tail(t);
}

void TaskGenerateStruct::generate_ctor(vsc::dm::IDataTypeStruct *t) {
    TaskGenerateStructCtor(m_gen, m_out).generate(t);
}

void TaskGenerateStruct::generate_dtor(vsc::dm::IDataTypeStruct *t) {
    TaskGenerateStructDtor(m_gen, m_out).generate(t);
}

void TaskGenerateStruct::generate_init(vsc::dm::IDataTypeStruct *t) {
    TaskGenerateStructInit(m_gen, 0, m_out).generate(t);
}

void TaskGenerateStruct::generate_create_assign(vsc::dm::IDataTypeStruct *t) {
    TaskGenerateStructAssign(m_gen, m_out).generate(t);
}

void TaskGenerateStruct::generate_create_default(vsc::dm::IDataTypeStruct *t) {
    m_out->println("static function %s create_default();",
        m_gen->getNameMap()->getName(t).c_str());
    m_out->inc_ind();
    m_out->println("%s ret = new();",
        m_gen->getNameMap()->getName(t).c_str());
    m_out->println("return ret;");
    m_out->dec_ind();
    m_out->println("endfunction");
}

void TaskGenerateStruct::generate_create_init(vsc::dm::IDataTypeStruct *t) {
    TaskGenerateStructCreateInit(m_gen, m_out).generate(t);
}

void TaskGenerateStruct::generate_tail(vsc::dm::IDataTypeStruct *t) {
    m_out->dec_ind();
    m_out->println("endclass");
}

void TaskGenerateStruct::generate_fields(vsc::dm::IDataTypeStruct *t) {
    TaskGenerateStructFields(m_gen, m_out).generate(t);
}

void TaskGenerateStruct::generate_constraints(vsc::dm::IDataTypeStruct *t) {
    GenRefExprExecModel genref(
        m_gen,
        t,
        "this",
        false);
    TaskGenerateStructConstraints(m_gen, &genref, m_out).generate(t);
}

void TaskGenerateStruct::generate_execs(vsc::dm::IDataTypeStruct *t) {
    using std::get;
    if (dynamic_cast<arl::dm::IDataTypeArlStruct *>(t)) {
        GenRefExprExecModel genref(
            m_gen,
            t,
            "this",
            false);

        std::vector<std::tuple<arl::dm::ExecKindT,bool,bool,std::string>> exec_t = {
            {arl::dm::ExecKindT::PreSolve, false, false, "pre_solve"},
            {arl::dm::ExecKindT::PostSolve, false, true, "post_solve"},
        };

        for (auto it=exec_t.begin(); it!=exec_t.end(); it++) {
            const std::vector<arl::dm::ITypeExecUP> &execs = 
                dynamic_cast<arl::dm::IDataTypeArlStruct *>(t)->getExecs(get<0>(*it));
        
            if (execs.size()) {
                TaskGenerateExecBlock(m_gen, &genref, m_out).generate(
                    execs, 
                    get<1>(*it),  // task
                    get<2>(*it),  // executor
                    get<3>(*it)); // name
            }
            TaskGenerateStructDoSolveExec(m_gen, m_out).generate(
                t, 
                get<2>(*it),
                get<3>(*it));
        }

    }
}

void TaskGenerateStruct::generate_methods(vsc::dm::IDataTypeStruct *t) {
    DEBUG_ENTER("generate_methods");
    DEBUG_LEAVE("generate_methods");
}


}
}
}
}
