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

    m_out->println("class activity_%p extends activity;", activity);
    m_out->inc_ind();
    m_out->println("%s actor;", m_gen->getActorName().c_str());
    m_out->println("function new(%s actor);", m_gen->getActorName().c_str());
    m_out->inc_ind();
    m_out->println("this.actor = actor;");
    m_out->dec_ind();
    m_out->println("endfunction");
    m_out->println("");
    m_out->println("virtual task run();");
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

    DEBUG_LEAVE("generate");
}

void TaskGenerateActivity::visitDataTypeActivitySequence(arl::dm::IDataTypeActivitySequence *t) {
    bool new_scope = (m_depth > 1);

    for (std::vector<arl::dm::ITypeFieldActivityUP>::const_iterator
        it=t->getActivities().begin();
        it!=t->getActivities().end(); it++) {
        (*it)->accept(m_this);
    }
}

void TaskGenerateActivity::visitDataTypeActivityTraverseType(arl::dm::IDataTypeActivityTraverseType *t) {

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
    run->println("int unsigned comp_id;");
    run->println("%s = new();", varname);
    run->println("%s.do_pre_solve();", varname);
    run->indent();
    run->write("if (!std::randomize(%s", varname);

    // Add in extra variables
    run->write(", comp_id");

    // Option
    bool include_with = true;
    if (include_with) {
        run->write(") with {\n");
        run->inc_ind();
        run->println("comp_id inside {0};");
        run->dec_ind();
        run->indent();
        run->write("}) begin\n");
    } else {
        run->write(") begin\n");
    }
    run->inc_ind();
    run->dec_ind();
    run->println("end");
    run->println("if (!$cast(%s.comp, actor.comp_l[comp_id])) begin", varname);
    run->inc_ind();
    run->println("$display(\"Failed to cast component\");");
    run->dec_ind();
    run->println("end");
    run->println("$cast(executor, %s.comp.get_default_executor());", varname);
    run->println("%s.do_post_solve(executor);", varname);
    if (t->getTarget()->activities().size()) {
        // TODO: invoke activity
    } else if (t->getTarget()->getExecs(arl::dm::ExecKindT::Body).size()) {
        run->println("%s.body(executor);", varname);
    }
    run->println("%s.dtor();", varname);
    run->dec_ind();
    run->println("end");
}

}
}
}
}
