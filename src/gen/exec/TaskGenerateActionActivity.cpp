/*
 * TaskGenerateActionActivity.cpp
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
#include "TaskGenerateActionActivity.h"
#include "TaskGenerateConstraint.h"
#include "ActivityInfo.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateActionActivity::TaskGenerateActionActivity(
    TaskGenerate                *gen,
    IGenRefExpr                 *genref,
    IOutput                     *out,
    vsc::dm::IDataTypeStruct    *t) : m_dbg(0), m_gen(gen), 
    m_genref(genref), m_id(0), m_out(out) {
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateActionActivity", gen->getDebugMgr());
    m_action_t = dynamic_cast<arl::dm::IDataTypeAction *>(t);
}

TaskGenerateActionActivity::~TaskGenerateActionActivity() {

}

void TaskGenerateActionActivity::generate(const std::vector<arl::dm::ITypeFieldActivityUP> &activities) {
//    arl::dm::IDataTypeActionActivity *activity = variant->info()->activity();
    DEBUG_ENTER("generate");
    OutputStr main_out(m_out->ind());

    m_out->println("virtual task run(activity_ctxt_c ctxt, int id=0);");
    m_out->inc_ind();
    m_out->println("case (id)");

    enter_activity((activities.size()>1)?ScopeT::Schedule:ScopeT::Sequence);

    for (std::vector<arl::dm::ITypeFieldActivityUP>::const_iterator
        it=activities.begin();
        it!=activities.end(); it++) {
        (*it)->accept(m_this);
    }

    leave_activity();

    m_out->println("default: begin");
    m_out->inc_ind();
    m_out->println("`ZSP_FATAL((\"Invalid activity id %%0d\", id));");
    m_out->dec_ind();
    m_out->println("end");
    m_out->println("endcase");
    m_out->dec_ind();
    m_out->println("endtask");
    m_out->println("");

    for (std::vector<OutputStrUP>::const_iterator
        it=m_out_l.begin();
        it!=m_out_l.end(); it++) {
        m_out->writes((*it)->getValue().c_str());
    }

    DEBUG_LEAVE("generate");
}

void TaskGenerateActionActivity::visitDataTypeActivityParallel(arl::dm::IDataTypeActivityParallel *t) {
    DEBUG_ENTER("visitDataTypeActivityParallel");

    // Create a closure that represents the entire 'parallel'
    // - new sub-activity
    // - 
    enter_activity(ScopeT::Parallel);
    for (std::vector<arl::dm::ITypeFieldActivityUP>::const_iterator
        it=t->getActivities().begin();
        it!=t->getActivities().end(); it++) {
        (*it)->accept(m_this);
    }

    leave_activity();

    // TODO: must detect and handle replicate inside
    DEBUG_LEAVE("visitDataTypeActivityParallel");
}

void TaskGenerateActionActivity::visitDataTypeActivitySequence(arl::dm::IDataTypeActivitySequence *t) {
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

void TaskGenerateActionActivity::visitDataTypeActivityTraverse(arl::dm::IDataTypeActivityTraverse *t) {
    DEBUG_ENTER("visitDataTypeActivityTraverse");
    IOutput *run = m_out_activity->run();
    ActivityVariant *variant = m_variant_s.back();

    std::string varname = m_genref->genRval(t->getTarget());

    run->println("// Traverse %s", varname.c_str());
    run->println("begin");
    run->inc_ind();

    if (t->getWithC()) {
        run->println("activity_%p activity;", t);
    } else {
        run->println("activity_traverse_c #(%s)::do_run(%s, ctxt);",
            m_gen->getNameMap()->getName(t->getTarget()).c_str(),
            varname.c_str());
    }
    // run->println("%s = new();", varname.c_str());
    // // Actually want to specify 'comp' as the launching scope
    // run->println("activity = new(actor, parent_comp, %s);", varname.c_str());
    // run->println("activity.run();");
//     run->println("%s.do_pre_solve();", varname.c_str());
//     run->indent();
//     run->write("if (%s.randomize()", varname.c_str());

//     bool include_with = true;
//     if (include_with) {
//         run->write(" with {\n");
//         run->inc_ind();
//         if (variant->info()->action()) {
//             run->println("parent_comp_id == self.comp_id;");
//         } else {
//             run->println("parent_comp_id == 0;");
//         }
//         arl::dm::IDataTypeAction *action_t = variant->info()->action();
//         const arl::eval::IComponentTreeData::TypeM &comp_m = 
//             m_gen->getCompTreeData()->getTypeMap(action_t->getComponentType());

//         for (arl::eval::IComponentTreeData::TypeM::const_iterator
//             it=comp_m.begin();
//             it!=comp_m.end(); it++) {
//             run->indent();
//             if (it->parent_id.size() == 1) {
//                 run->write("(parent_comp_id == %d) -> ", it->parent_id.at(0));
//             } else {
//                 run->write("(parent_comp_id inside {");
//                 for (uint32_t i=0; i<it->parent_id.size(); i++) {
//                     if (i) {
//                         run->write(", ");
//                     }
//                     run->write("%d", it->parent_id.at(i));
//                 }
//                 run->write("}) -> ");
//             }
//             if (it->child_id.size() == 1) {
//                 run->write("comp_id == %d;\n", it->child_id.at(0));
//             } else {
//                 run->write("comp_id inside {");
//                 for (uint32_t i=0; i<it->child_id.size(); i++) {
//                     if (i) {
//                         run->write(", ");
//                     }
//                     run->write("%d", it->child_id.at(i));
//                 }
//                 run->write("};\n");
//             }
//         }

//         if (t->getWithC()) {
//             TaskGenerateConstraint(m_gen, m_genref, run).generate(t->getWithC());
//         }
//         run->dec_ind();
//         run->indent();
//         run->write("}) begin\n");
//     } else {
//         run->write(") begin\n");
//     }
//     run->inc_ind();
//     // TODO: error handling
//     run->dec_ind();
//     run->println("end");
// //    run->println("$display(\"parent_comp_id=%%0d ; comp_id=%%0d\", %s.parent_comp_id, %s.comp_id);",
// //        varname.c_str(), varname.c_str());

//     run->println("foreach (actor.listeners[i]) begin");
//     run->inc_ind();
//     run->println("actor.listeners[i].enter_traverse(%s);", varname.c_str());
//     run->dec_ind();
//     run->println("end");

//     // TODO: handle calling exec, activity, etc

//     run->println("foreach (actor.listeners[i]) begin");
//     run->inc_ind();
//     run->println("actor.listeners[i].leave_traverse(%s);", varname.c_str());
//     run->dec_ind();
//     run->println("end");

    run->println("%s.drop();", varname.c_str());
    run->dec_ind();
    run->println("end");

    DEBUG_LEAVE("visitDataTypeActivityTraverse");
}

void TaskGenerateActionActivity::visitDataTypeActivityTraverseType(arl::dm::IDataTypeActivityTraverseType *t) {
    DEBUG_ENTER("visitDataTypeActivityTraverseType");

    // TODO: if this activity has an inline 'with', then we must use 
    // an existing activity specialization (that embeds the 'with').
    // 
    // The good news in that situation is that the dependencies can either be
    // - determined to be state variables because they are from the containing action
    //   or a predecessor.
    // - determined to be from a successor activity and, thus, covered by static lookahead
    //

    // TODO: handle 'with' constraint
    // TODO: handle 'init' 
    std::string with_c = "null";
    std::string init = "null";

    out()->println("ctxt.add(activity_traverse_c #(%s)::mk(null, %s, %s));",
        m_gen->getNameMap()->getName(t->getTarget()).c_str(),
        with_c.c_str(),
        init.c_str());

//    run->println("// Traverse action %s", t->getTarget()->name().c_str());
//    run->println("begin");
//    run->inc_ind();
//    if (t->getWithC()) {
//        run->println("activity_%p activity = new(actor, parent_comp);", t);
//    } else {
//        run->println("activity_traverse_c #(%s) activity = new(actor, parent_comp);", 
//            m_gen->getNameMap()->getName(t->getTarget()).c_str());
//    }
//    run->println("activity.run();");
//    run->println("activity.drop(); // TODO: defer?");
//    run->dec_ind();
//    run->println("end");

    // ActionActivityVariant *variant = m_variant_s.back();
    // char varname[64];
    // snprintf(varname, sizeof(varname), "%s_%p", 
    //     m_gen->getNameMap()->getName(t->getTarget()).c_str(), t);
    // m_out_activity->decl()->println("%s %s;", 
    //     m_gen->getNameMap()->getName(t->getTarget()).c_str(), varname);
    // run->println("// Traverse action %s", t->getTarget()->name().c_str());
    // run->println("begin");
    // run->inc_ind();
    // run->println("executor_t executor;");
    // run->println("%s = new();", varname);
    // run->println("%s.do_pre_solve();", varname);
    // run->indent();
    // run->write("if (!%s.randomize()", varname);

//     // Option
//     bool include_with = true;
//     if (include_with) {
//         run->write(" with {\n");
//         run->inc_ind();
//         if (variant->info()->action()) {
//             run->println("parent_comp_id == self.comp_id;");
//         } else {
//             run->println("parent_comp_id == 0;");
//         }
//         arl::dm::IDataTypeAction *action_t = t->getTarget();
//         const arl::eval::IComponentTreeData::TypeM &comp_m = 
//             m_gen->getCompTreeData()->getTypeMap(action_t->getComponentType());

//         for (arl::eval::IComponentTreeData::TypeM::const_iterator
//             it=comp_m.begin();
//             it!=comp_m.end(); it++) {
//             run->indent();
//             if (it->parent_id.size() == 1) {
//                 run->write("(parent_comp_id == %d) -> ", it->parent_id.at(0));
//             } else {
//                 run->write("(parent_comp_id inside {");
//                 for (uint32_t i=0; i<it->parent_id.size(); i++) {
//                     if (i) {
//                         run->write(", ");
//                     }
//                     run->write("%d", it->parent_id.at(i));
//                 }
//                 run->write("}) -> ");
//             }
//             if (it->child_id.size() == 1) {
//                 run->write("comp_id == %d;\n", it->child_id.at(0));
//             } else {
//                 run->write("comp_id inside {");
//                 for (uint32_t i=0; i<it->child_id.size(); i++) {
//                     if (i) {
//                         run->write(", ");
//                     }
//                     run->write("%d", it->child_id.at(i));
//                 }
//                 run->write("};\n");
//             }
//         }

//         if (t->getWithC()) {
//             m_genref->pushInline(t->getTarget());
//             TaskGenerateConstraint(m_gen, m_genref, run).generate(t->getWithC());
//             m_genref->popInline();
//         }
// //        run->println("%s.comp_id inside {0};", varname);
//         run->dec_ind();
//         run->indent();
//         run->write("}) begin\n");
//     } else {
//         run->write(") begin\n");
//     }
//     run->inc_ind();
//     run->println("$display(\"Error: failed to randomize action\");");
//     run->println("$finish;");
//     run->dec_ind();
//     run->println("end");
// //    run->println("$display(\"parent_comp_id=%%0d ; comp_id=%%0d\", %s.parent_comp_id, %s.comp_id);",
// //        varname, varname);
//     run->println("if (!$cast(%s.comp, actor.comp_l[%s.comp_id])) begin", 
//         varname, varname);
//     run->inc_ind();
//     run->println("$display(\"Failed to cast component\");");
//     run->dec_ind();
//     run->println("end");
//     run->println("if (%s.comp == null) begin", varname);
//     run->inc_ind();
//     run->println("$display(\"Component @ %%0d is null\", %s.comp_id);", varname);
//     run->dec_ind();
//     run->println("end");
//     run->println("$cast(executor, %s.comp.get_default_executor());", varname);
//     run->println("%s.do_post_solve(executor);", varname);
//     run->println("");
//     run->println("foreach (actor.listeners[i]) begin");
//     run->inc_ind();
//     run->println("actor.listeners[i].enter_traverse(%s);", varname);
//     run->dec_ind();
//     run->println("end");
//     if (t->getTarget()->activities().size()) {
//         arl::dm::IDataTypeActionActivity *activity = t->getTarget()->activities().at(0)->getDataTypeT<arl::dm::IDataTypeActionActivity>();
//         DEBUG("activity: %p", activity);
//         ActionActivityVariant *variant = m_variant_s.back()->getVariant(activity);
//         DEBUG("variant: %p", variant);
//         // TODO: invoke activity
//         run->println("begin");
//         run->inc_ind();
//         if (variant->info()->action()) {
//             run->println("%s activity = new(actor, %s);",
//                 m_gen->getNameMap()->getName(variant->info()->activity()).c_str(),
//                 varname);
//         } else {
//             run->println("%s activity = new(actor);",
//                 m_gen->getNameMap()->getName(variant->info()->activity()).c_str());
//         }

//         if (variant->info()->action()) {
//             run->println("activity.run();", varname);
//         } else {
//             run->println("activity.run();");
//         }
//         run->dec_ind();
//         run->println("end");
//     } else if (t->getTarget()->getExecs(arl::dm::ExecKindT::Body).size()) {
//         run->println("%s.body(executor);", varname);
//     }
//     run->println("");
//     run->println("foreach (actor.listeners[i]) begin");
//     run->inc_ind();
//     run->println("actor.listeners[i].leave_traverse(%s);", varname);
//     run->dec_ind();
//     run->println("end");
//     run->println("");
//     run->println("%s.drop();", varname);
//     run->dec_ind();
//     run->println("end");
    DEBUG_LEAVE("visitDataTypeActionActivityTraverseType");
}

void TaskGenerateActionActivity::enter_activity(ScopeT kind) {
    OutputStr *out = new OutputStr("        ");

    int32_t id = m_out_l.size();

    // Add an entry to the dispatch task
    if (id == 0) {
        m_out->println("0: run_0(activity_ctxt_%s_c::mk(ctxt));",
            (kind == ScopeT::Schedule)?"sched":"seq");
    } else {
        m_out->println("%d: run_%d(activity_ctxt_%s_c::mk(ctxt));", 
            m_out_l.size(), 
            m_out_l.size(),
            (kind == ScopeT::Schedule)?"sched":
                (kind == ScopeT::Parallel)?"par":"seq");

        // Add a call to the activity to the current context
        m_out_l.back()->println("ctxt.add(activity_proxy_c #(%s)::mk(this, %d));",
            m_gen->getNameMap()->getName(m_action_t).c_str(), id);
    }

    // Establish a new function scope
    m_scope_s.push_back(kind);

    out->println("task run_%d(activity_ctxt_c ctxt);", m_out_l.size());
    out->inc_ind();
    m_out_l.push_back(OutputStrUP(out));
    m_out_s.push_back(out);

    // if (kind == ScopeT::Schedule) {
    //     out->println("activity_ctxt_sched_c sctxt = new(ctxt);");
    // } else if (kind == ScopeT::Parallel) {
    //     out->println("activity_ctxt_par_c sctxt = new(ctxt);");
    // } else if (kind == ScopeT::Sequence) {
    //     out->println("activity_ctxt_c sctxt = ctxt;");
    // }

}

void TaskGenerateActionActivity::leave_activity() {
    ScopeT kind = m_scope_s.back();

    out()->println("// Perform end-of-scope processing");
    out()->println("ctxt.end_scope();");

    out()->dec_ind();
    out()->println("endtask");
    m_out_s.pop_back();
    m_scope_s.pop_back();
}

}
}
}
}