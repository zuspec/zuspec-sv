/*
 * TaskGenerateRegGroup.cpp
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
#include "TaskGenerateExpr.h"
#include "TaskGenerateFunction.h"
#include "TaskGenerateRegGroup.h"

namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateRegGroup::TaskGenerateRegGroup(
    TaskGenerate        *gen,
    IOutput             *out) : m_gen(gen), m_out(out) {
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateRegGroup", gen->getDebugMgr());
}

TaskGenerateRegGroup::~TaskGenerateRegGroup() {

}

void TaskGenerateRegGroup::generate(vsc::dm::IDataTypeStruct *t) {
    m_out->println("class %s extends reg_group_c;",
        m_gen->getNameMap()->getName(t).c_str());
    m_out->inc_ind();
    m_out->println("static %s prv_inst;",
        m_gen->getNameMap()->getName(t).c_str());
    m_phase = PhaseE::Decl;
    for (std::vector<vsc::dm::ITypeFieldUP>::const_iterator
        it=t->getFields().begin();
        it!=t->getFields().end(); it++) {
        (*it)->accept(m_this);
    }
    m_out->println("");
    m_phase = PhaseE::Ctor;
    m_out->println("function new(executor_base exec_b);");
    m_out->inc_ind();
    m_out->println("super.new();");
    for (std::vector<vsc::dm::ITypeFieldUP>::const_iterator
        it=t->getFields().begin();
        it!=t->getFields().end(); it++) {
        (*it)->accept(m_this);
    }
    m_out->println("");
    m_out->println("map_registers(exec_b);");
    m_out->dec_ind();
    m_out->println("endfunction");
    m_out->println("");

    GenRefExprExecModel genref(
        m_gen,
        t,
        "this",
        false);
    m_genref = &genref;
    std::string prefix = t->name() + "::";
    for (std::vector<arl::dm::IDataTypeFunction *>::const_iterator
        it=m_gen->getContext()->getDataTypeFunctions().begin();
        it!=m_gen->getContext()->getDataTypeFunctions().end(); it++) {
        if ((*it)->name().find(prefix) == 0) {
            DEBUG("Function: %s", (*it)->name().c_str());
            (*it)->setFlags(arl::dm::DataTypeFunctionFlags::Solve);
            TaskGenerateFunction(m_gen, &genref, m_out).generate(*it, true);
            m_out->println("");
        }
    }

    m_out->println("static function %s inst(executor_base exec_b);",
        m_gen->getNameMap()->getName(t).c_str());
    m_out->inc_ind();
    m_out->println("if (prv_inst == null) begin");
    m_out->inc_ind();
    m_out->println("prv_inst = new(exec_b);",
        m_gen->getNameMap()->getName(t).c_str());
    m_out->dec_ind();
    m_out->println("end");
    m_out->println("return prv_inst;");
    m_out->dec_ind();
    m_out->println("endfunction");
    m_out->dec_ind();
    m_out->println("endclass");

}

void TaskGenerateRegGroup::visitTypeFieldReg(arl::dm::ITypeFieldReg *f) {
    DEBUG_ENTER("visitTypeFieldReg %s", f->name().c_str());
    switch (m_phase) {
        case PhaseE::Decl:
            m_out->println("reg_field_c         %s;", f->name().c_str());
            break;
        case PhaseE::Ctor:
            m_out->println("%s = new(\"%s\");", 
                f->name().c_str(), f->name().c_str());
            m_out->println("fields.push_back(%s);", f->name().c_str());
            break;
    }
    DEBUG_LEAVE("visitTypeFieldReg");
}

void TaskGenerateRegGroup::visitTypeFieldRegGroup(arl::dm::ITypeFieldRegGroup *f) {
    DEBUG_ENTER("visitTypeFieldRegGroup");
    switch (m_phase) {
        case PhaseE::Decl:
            m_out->println("reg_group_field_c #(%s) %s;",
                m_gen->getNameMap()->getName(f->getDataType()).c_str(),
                f->name().c_str());
            break;
        case PhaseE::Ctor:
            m_out->println("%s = new(\"%s\", %s::inst(exec_b));", 
                f->name().c_str(), f->name().c_str(),
                m_gen->getNameMap()->getName(f->getDataType()).c_str());
            m_out->println("fields.push_back(%s);", f->name().c_str());
            break;
    }
    DEBUG_LEAVE("visitTypeFieldRegGroup");
}

void TaskGenerateRegGroup::visitTypeFieldRegGroupArr(arl::dm::ITypeFieldRegGroupArr *f) {
    DEBUG_ENTER("visitTypeFieldRegGroupArr");
    switch (m_phase) {
        case PhaseE::Decl: {
            vsc::dm::IDataTypeArray *array_t = f->getDataTypeT<vsc::dm::IDataTypeArray>();
            m_out->println("reg_group_field_arr_c #(%s) %s;",
                m_gen->getNameMap()->getName(array_t->getElemType()).c_str(),
                f->name().c_str());
        } break;
        case PhaseE::Ctor: {
            vsc::dm::IDataTypeArray *arr_t = f->getDataTypeT<vsc::dm::IDataTypeArray>();
            m_out->println("%s = new(\"%s\", %d, %s::inst(exec_b));", 
                f->name().c_str(), 
                f->name().c_str(),
                arr_t->getSize(),
                m_gen->getNameMap()->getName(arr_t->getElemType()).c_str());
            m_out->println("fields.push_back(%s);", f->name().c_str());
        } break;
    }
    DEBUG_LEAVE("visitTypeFieldRegGroupArr");
}

dmgr::IDebug *TaskGenerateRegGroup::m_dbg = 0;

}
}
}
}
