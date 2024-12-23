/*
 * TaskGenerateActorPkgPrv.cpp
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
#include "zsp/arl/dm/IDataTypeActivityTraverseType.h"
#include "zsp/arl/dm/ITypeFieldActivity.h"
#include "gen/Output.h"
#include "gen/exec/TaskBuildActivityInfo.h"
#include "CustomGenAddrHandle.h"
#include "CustomGenAddrRegion.h"
#include "CustomGenAddrRegionTransparent.h"
#include "CustomGenMemRwCall.h"
#include "CustomGenPrintCall.h"
#include "CustomGenRegAccessCall.h"
#include "CustomGenRegGetHandle.h"
#include "GenRefExprExecModel.h"
#include "ICustomGen.h"
#include "TaskGenerateActorPkgPrv.h"
#include "TaskGenerateActivity.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateActorPkgPrv::TaskGenerateActorPkgPrv(
    dmgr::IDebugMgr                 *dmgr,
    arl::dm::IContext               *ctxt,
    arl::eval::IFactory             *eval_f,
    arl::dm::IDataTypeComponent     *comp_t,
    arl::dm::IDataTypeAction        *action_t,
    std::ostream                    *out) : TaskGenerate(dmgr, ctxt, eval_f, out),
    m_comp_t(comp_t), m_action_t(action_t) {
    m_dbg = 0;
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateActorPkgPrv", dmgr);
}

TaskGenerateActorPkgPrv::~TaskGenerateActorPkgPrv() {

}

bool TaskGenerateActorPkgPrv::generate() {
    std::string actor = "pss_top__Entry";
    OutputUP out(new Output(m_out, false));

    attach_custom_gen();

    m_actor_name = "actor_t";

    // First, build out the component-tree data structure
    m_comptree_data = arl::eval::IComponentTreeDataUP(
        m_eval_f->mkComponentTreeData(m_comp_t));

    arl::dm::IDataTypeActivitySequenceUP root_activity(
        m_ctxt->mkDataTypeActivitySequence());

    root_activity->addActivity(
        m_ctxt->mkTypeFieldActivity("", 
            m_ctxt->mkDataTypeActivityTraverseType(m_action_t, 0), true), true);

    out->println("`include \"zsp_sv_macros.svh\"");
    out->println("package %s_prv;", actor.c_str());
    out->inc_ind();
    out->println("import zsp_sv::*;");
    out->println("import pss_types::*;");
    out->println("");
    out->println("typedef class %s_actor;", actor.c_str());
    out->println("typedef %s_actor actor_t;", actor.c_str());

    // TODO: generate content

    std::vector<ActivityInfoUP> activity_info = TaskBuildActivityInfo(
        m_dmgr,
        m_namemap.get()).build(
            0,
            root_activity.get());
    
    DEBUG("activity_info: %d", activity_info.size());

    for (std::vector<ActivityInfoUP>::const_iterator
        it=activity_info.begin();
        it!=activity_info.end(); it++) {

        for (std::vector<ActivityVariantUP>::const_iterator
            v_it=(*it)->variants().begin(); 
            v_it!=(*it)->variants().end(); v_it++) {
            out->println("typedef class activity_%p;", 
                v_it->get()->info()->activity());
        }
    }

    for (std::vector<ActivityInfoUP>::const_iterator
        it=activity_info.begin();
        it!=activity_info.end(); it++) {

        DEBUG("variants: %d", (*it)->variants().size());

        for (std::vector<ActivityVariantUP>::const_iterator
            v_it=(*it)->variants().begin(); 
            v_it!=(*it)->variants().end(); v_it++) {

            out->println("");

            // TODO: temporary gen-ref
            GenRefExprExecModel genref(
                this,
                (*v_it)->info()->action(),
                "self",
                false);

            TaskGenerateActivity(
                this, 
                &genref, 
                out.get()
            ).generate(v_it->get());
        }
    }

    // Define the import API
#ifdef UNDEFINED
    out->println("");
    TaskGenerateImportApi(this, out.get()).generate(
        m_ctxt->getDataTypeFunctions());
    out->println("");
#endif

    // Define the actor
//     #(.comp_t(%s), .activity_t(activity_%p));", 
    out->println("class %s_actor extends actor_c;", actor.c_str());
    out->inc_ind();
    out->println("%s comp_tree;", getNameMap()->getName(m_comp_t).c_str());
    out->println("pss_import_api api;");
    out->println("executor_base_c default_executor;");
    out->println("");
    out->println("function new(pss_import_api api=null);");
    out->inc_ind();
    out->println("component_ctor_ctxt ctxt = new(this, 0);");
    out->println("super.new(\"<actor>\", null, null);");
    out->println("this.api = api;");
    out->println("this.default_executor = new(\"default_executor\", this);");
    out->println("comp_tree = new(\"pss_top\", ctxt, this);");
    out->dec_ind();
    out->println("endfunction");
    out->println("");
    out->println("virtual task run();");
    out->inc_ind();
    out->println("activity_%p root_activity = new(this, comp_tree);", root_activity.get());
    out->println("");
    out->println("comp_tree.init(this.default_executor);");
    out->println("comp_tree.do_init(this.default_executor);");
    out->println("");
    out->println("if (comp_tree.check()) begin");
    out->inc_ind();
    out->println("if (api == null) begin");
    out->inc_ind();
    out->println("api = new();");
    out->dec_ind();
    out->println("end");
    out->println("");
    out->println("comp_tree.start(this.default_executor);");
    out->println("");
    out->println("foreach(listeners[i]) begin");
    out->inc_ind();
    out->println("listeners[i].enter_actor(this);");
    out->dec_ind();
    out->println("end");
    out->println("");
    out->println("root_activity.run();");
    out->println("");
    out->println("foreach(listeners[i]) begin");
    out->inc_ind();
    out->println("listeners[i].leave_actor(this);");
    out->dec_ind();
    out->println("end");
    out->dec_ind();
    out->println("end else begin");
    out->inc_ind();
    out->println("$display(\"Error: initialization check failed\");");
    out->dec_ind();
    out->println("end");
    out->println("");
    out->dec_ind();
    out->println("endtask");
    out->println("");
    out->println("virtual function pss_import_api get_api();");
    out->inc_ind();
    out->println("return api;");
    out->dec_ind();
    out->println("endfunction");
    out->println("");
    out->println("virtual function executor_base get_default_executor();");
    out->inc_ind();
    out->println("return default_executor;");
    out->dec_ind();
    out->println("endfunction");
    out->println("");
    out->println("virtual function backend_api get_backend();");
    out->inc_ind();
    out->println("return api;");
    out->dec_ind();
    out->println("endfunction");


    out->dec_ind();
    out->println("endclass");

    out->dec_ind();
    out->println("endpackage");

//    (*m_out) << std::endl;
//    (*m_out) << "// Import the public package\n";

    out->close();

    return true;
}

}
}
}
}
