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
#include "TaskGenerateActionActivity.h"
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

void TaskGenerateAction::generate(vsc::dm::IDataTypeStruct *t) {
    DEBUG_ENTER("generate %s", t->name().c_str());
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
    generate_activity(t);
    generate_methods(t);
    generate_tail(t);
    DEBUG_LEAVE("generate %s", t->name().c_str());
}

void TaskGenerateAction::generate_head(vsc::dm::IDataTypeStruct *t) {
    arl::dm::IDataTypeAction *action_t = dynamic_cast<arl::dm::IDataTypeAction *>(t);
    m_out->println("typedef class %s;", 
        m_gen->getNameMap()->getName(action_t->getComponentType()).c_str());
    m_out->println("class %s extends action_c;", m_gen->getNameMap()->getName(t).c_str());
    m_out->inc_ind();
    m_out->println("`zsp_action_util_begin(%s, %s)", 
        m_gen->getNameMap()->getName(t).c_str(),
        m_gen->getNameMap()->getName(action_t->getComponentType()).c_str());
    m_out->println("`zsp_action_util_end");
}

void TaskGenerateAction::generate_fields(vsc::dm::IDataTypeStruct *t) {
    TaskGenerateActionFields(m_gen, m_out).generate(t);
}

void TaskGenerateAction::generate_constraints(vsc::dm::IDataTypeStruct *t) {
    arl::dm::IDataTypeAction *action_t = dynamic_cast<arl::dm::IDataTypeAction *>(t);
#ifdef UNDEFINED
    const arl::eval::IComponentTreeData::TypeM &comp_m = 
        m_gen->getCompTreeData()->getTypeMap(action_t->getComponentType());

    m_out->println("constraint __comp_id_c {");
    m_out->inc_ind();
    for (arl::eval::IComponentTreeData::TypeM::const_iterator
        it=comp_m.begin();
        it!=comp_m.end(); it++) {
        m_out->indent();
        if (it->parent_id.size() == 1) {
            m_out->write("(parent_comp_id == %d) -> ", it->parent_id.at(0));
        } else {
            m_out->write("(parent_comp_id inside {");
            for (uint32_t i=0; i<it->parent_id.size(); i++) {
                if (i) {
                    m_out->write(", ");
                }
                m_out->write("%d", it->parent_id.at(i));
            }
            m_out->write("}) -> ");
        }
        if (it->child_id.size() == 1) {
            m_out->write("comp_id == %d;\n", it->child_id.at(0));
        } else {
            m_out->write("comp_id inside {");
            for (uint32_t i=0; i<it->child_id.size(); i++) {
                if (i) {
                    m_out->write(", ");
                }
                m_out->write("%d", it->child_id.at(i));
            }
            m_out->write("};\n");
        }
    }
    m_out->dec_ind();
    m_out->println("}");
#endif

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

void TaskGenerateAction::generate_activity(vsc::dm::IDataTypeStruct *t) {
    DEBUG_ENTER("generate_activity");
    arl::dm::IDataTypeAction *action_t = dynamic_cast<arl::dm::IDataTypeAction *>(t);

    if (action_t->activities().size()) {
        GenRefExprExecModel genref(
            m_gen,
            t,
            "this",
            false);
        TaskGenerateActionActivity(m_gen, &genref, m_out).generate(action_t->activities());
    }

    DEBUG_LEAVE("generate_activity");
}

void TaskGenerateAction::generate_methods(vsc::dm::IDataTypeStruct *t) {
    DEBUG_ENTER("generate_methods");
    arl::dm::IDataTypeAction *action_t = dynamic_cast<arl::dm::IDataTypeAction *>(t);

    // m_out->println("virtual function void set_component(component_c comp);");
    // m_out->inc_ind();
    // m_out->println("$cast(this.comp, comp);");
    // m_out->dec_ind();
    // m_out->println("endfunction");
    // m_out->println("");
    // m_out->println("virtual function component_c get_component();");
    // m_out->inc_ind();
    // m_out->println("return this.comp;");
    // m_out->dec_ind();
    // m_out->println("endfunction");
    // m_out->println("");

    // m_out->println("virtual task run();");
    // m_out->inc_ind();
    // if (action_t->activities().size()) {
    //     GenRefExprExecModel genref(
    //         m_gen,
    //         t,
    //         "this",
    //         false);

    //     if (action_t->activities().size() > 1) {
    //         DEBUG("TODO: handle schedule");
    //     } else {
    //         m_out->println("activity_%p activity = new(actor, this);",
    //             action_t->activities().at(0).get());
    //         m_out->println("activity.run();");
    //         m_out->println("activity.drop();");
    //     }
    // } else {
    //     const std::vector<arl::dm::ITypeExecUP> &execs = 
    //         action_t->getExecs(arl::dm::ExecKindT::Body);
    // }
    // m_out->dec_ind();
    // m_out->println("endtask");

    DEBUG_LEAVE("generate_methods");
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
