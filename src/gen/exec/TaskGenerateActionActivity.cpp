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
    TaskGenerate            *gen,
    IGenRefExpr             *genref,
    IOutput                 *out) : m_dbg(0), m_gen(gen), m_genref(genref), m_out(out) {
    DEBUG_INIT("Zsp::sv::gen::exec::TaskGenerateActionActivity", gen->getDebugMgr());
}

TaskGenerateActionActivity::~TaskGenerateActionActivity() {

}

void TaskGenerateActionActivity::generate(const std::vector<arl::dm::ITypeFieldActivityUP> &activities) {
//    arl::dm::IDataTypeActionActivity *activity = variant->info()->activity();
    DEBUG_ENTER("generate");
    OutputActivityScope out_activity(m_out);
    m_out_activity = &out_activity;

    if (activities.size() > 1) {
        DEBUG("TODO: handle schedule");
    } else {
        activities.at(0)->accept(m_this);
    }

    out_activity.apply(m_out);

    DEBUG_LEAVE("generate");
}

void TaskGenerateActionActivity::visitDataTypeActivityParallel(arl::dm::IDataTypeActivityParallel *t) {
    DEBUG_ENTER("visitDataTypeActivityParallel");
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
    } else if (variant->info()->action()->activities().size()) {
        run->println("activity_traverse_compound_c #(%s,%s) activity;", variant->info()->action());
    } else {
        run->println("activity_traverse_c #(%s) activity;",
            m_gen->getNameMap()->getName(variant->info()->action()).c_str());
    }
    run->println("%s = new();", varname.c_str());
    // Actually want to specify 'comp' as the launching scope
    run->println("activity = new(ctor, parent_comp, %s);", varname.c_str());
    run->println("activity.run();");
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

    run->println("%s.dtor();", varname.c_str());
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

    IOutput *run = m_out_activity->run();

    run->println("// Traverse action %s", t->getTarget()->name().c_str());
    run->println("begin");
    run->inc_ind();
    if (t->getWithC()) {
        run->println("activity_%p activity = new(actor, parent_comp);", t);
    } else {
        run->println("activity_traverse_c #(%s) activity = new(actor, parent_comp);", 
            m_gen->getNameMap()->getName(t->getTarget()).c_str());
    }
    run->println("activity.run();");
    run->println("activity.dtor();");
    run->dec_ind();
    run->println("end");

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
//     run->println("%s.dtor();", varname);
//     run->dec_ind();
//     run->println("end");
    DEBUG_LEAVE("visitDataTypeActionActivityTraverseType");
}

}
}
}
}
