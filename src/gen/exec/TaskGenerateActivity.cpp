/*
 * TaskGenerateActivity.cpp
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
#include "TaskGenerateActivity.h"
#include "ActivityInfo.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateActivity::TaskGenerateActivity(
    TaskGenerate            *gen,
    IGenRefExpr             *genref,
    IOutput                 *out) : m_dbg(0), m_gen(gen), m_genref(genref), m_out(out) {
    DEBUG_INIT("Zsp::sv::gen::exec::TaskGenerateActivity", gen->getDebugMgr());
}

TaskGenerateActivity::~TaskGenerateActivity() {

}

void TaskGenerateActivity::generate(ActivityVariant *variant) {
    arl::dm::IDataTypeActivity *activity = variant->info()->activity();
    DEBUG_ENTER("generate");

    m_variant_s.clear();
    m_variant_s.push_back(variant);

    m_out->println("class activity_%p extends activity;", activity);
    m_out->inc_ind();
//    m_out->println("%s actor;", m_gen->getActorName().c_str());
    m_out->println("function new();");
    m_out->inc_ind();
    m_out->dec_ind();
    m_out->println("endfunction");
    m_out->println("");
    if (variant->info()->action()) {
        m_out->println("virtual task run(%s actor, %s self);",
            m_gen->getActorName().c_str(),
            m_gen->getNameMap()->getName(variant->info()->action()).c_str());
    } else {
        m_out->println("virtual task run(%s actor);",
            m_gen->getActorName().c_str());
    }
    m_out->inc_ind();
    OutputActivityScope out(m_out);
    m_out_activity = &out;
    activity->accept(m_this);
    out.apply(m_out);
    m_out->dec_ind();
    m_out->println("endtask");
    m_out->println("");
    m_out->dec_ind();
    m_out->println("endclass");

    m_variant_s.pop_back();

    DEBUG_LEAVE("generate");
}

void TaskGenerateActivity::visitDataTypeActivitySequence(arl::dm::IDataTypeActivitySequence *t) {
    DEBUG_ENTER("visitDataTypeActivitySequence %p", t);
    bool new_scope = (m_depth > 1);
    if (!m_depth) {
        ActivityVariant *variant = m_variant_s.back()->getVariant(t);
        DEBUG("variant: %p", variant);
        m_variant_s.push_back(variant);
    }

    for (std::vector<arl::dm::ITypeFieldActivityUP>::const_iterator
        it=t->getActivities().begin();
        it!=t->getActivities().end(); it++) {
        (*it)->accept(m_this);
    }

    if (!m_depth) {
        m_variant_s.pop_back();
    }

    DEBUG_LEAVE("visitDataTypeActivitySequence");
}

void TaskGenerateActivity::visitDataTypeActivityTraverse(arl::dm::IDataTypeActivityTraverse *t) {
    DEBUG_ENTER("visitDataTypeActivityTraverse");
    IOutput *run = m_out_activity->run();

    std::string varname = m_genref->genRval(t->getTarget());

    run->println("// Traverse %s", varname.c_str());
    run->println("begin");
    run->inc_ind();
    run->println("%s = new();", varname.c_str());
    run->println("%s.do_pre_solve();", varname.c_str());
    run->indent();
    run->write("if (%s.randomize()", varname.c_str());

    bool include_with = true;
    if (include_with) {
        run->write(" with {\n");
        run->inc_ind();
        run->println("");
        run->dec_ind();
        run->indent();
        run->write("}) begin\n");
    } else {
        run->write(") begin\n");
    }
    run->inc_ind();
    // TODO: error handling
    run->dec_ind();
    run->println("end");

    // TODO: handle calling exec, activity, etc

    run->println("%s.dtor();", varname.c_str());
    run->dec_ind();
    run->println("end");

    DEBUG_LEAVE("visitDataTypeActivityTraverse");
}

void TaskGenerateActivity::visitDataTypeActivityTraverseType(arl::dm::IDataTypeActivityTraverseType *t) {
    DEBUG_ENTER("visitDataTypeActivityTraverseType");
    IOutput *run = m_out_activity->run();
    char varname[64];
    snprintf(varname, sizeof(varname), "%s_%p", 
        m_gen->getNameMap()->getName(t->getTarget()).c_str(), t);
    m_out_activity->decl()->println("%s %s;", 
        m_gen->getNameMap()->getName(t->getTarget()).c_str(), varname);
    run->println("// Traverse action %s", t->getTarget()->name().c_str());
    run->println("begin");
    run->inc_ind();
    run->println("executor_t executor;");
    run->println("%s = new();", varname);
    run->println("%s.do_pre_solve();", varname);
    run->indent();
    run->write("if (!%s.randomize()", varname);

    // Option
    bool include_with = true;
    if (include_with) {
        run->write(" with {\n");
        run->inc_ind();
        run->println("%s.comp_id inside {0};", varname);
        run->dec_ind();
        run->indent();
        run->write("}) begin\n");
    } else {
        run->write(") begin\n");
    }
    run->inc_ind();
    run->dec_ind();
    run->println("end");
    run->println("if (!$cast(%s.comp, actor.comp_l[%s.comp_id])) begin", 
        varname, varname);
    run->inc_ind();
    run->println("$display(\"Failed to cast component\");");
    run->dec_ind();
    run->println("end");
    run->println("$cast(executor, %s.comp.get_default_executor());", varname);
    run->println("%s.do_post_solve(executor);", varname);
    if (t->getTarget()->activities().size()) {
        arl::dm::IDataTypeActivity *activity = t->getTarget()->activities().at(0)->getDataTypeT<arl::dm::IDataTypeActivity>();
        DEBUG("activity: %p", activity);
        ActivityVariant *variant = m_variant_s.back()->getVariant(activity);
        DEBUG("variant: %p", variant);
        // TODO: invoke activity
        run->println("begin");
        run->inc_ind();
        run->println("%s activity = new;",
            m_gen->getNameMap()->getName(variant->info()->activity()).c_str());

        if (variant->info()->action()) {
            run->println("activity.run(actor, %s);", varname);
        } else {
            run->println("activity.run(actor);");
        }
        run->dec_ind();
        run->println("end");
    } else if (t->getTarget()->getExecs(arl::dm::ExecKindT::Body).size()) {
        run->println("%s.body(executor);", varname);
    }
    run->println("%s.dtor();", varname);
    run->dec_ind();
    run->println("end");
    DEBUG_LEAVE("visitDataTypeActivityTraverseType");
}

}
}
}
}
