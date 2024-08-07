/*
 * TaskGenerate.cpp
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
#include "zsp/arl/dm/IDataTypeActivitySequence.h"
#include "gen/NameMap.h"
#include "gen/OutputStr.h"
#include "gen/TaskBuildTypeCollection.h"
#include "gen/exec/TaskBuildActivityInfo.h"
#include "CustomGenPrintCall.h"
#include "TaskDefineType.h"
#include "TaskGenerate.h"
#include "TaskGenerateActivity.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerate::TaskGenerate(
    dmgr::IDebugMgr                 *dmgr,
    arl::dm::IContext               *ctxt,
    arl::dm::IDataTypeComponent     *comp_t,
    arl::dm::IDataTypeAction        *action_t,
    std::ostream                    *out) : m_dmgr(dmgr), m_ctxt(ctxt),
    m_comp_t(comp_t), m_action_t(action_t), m_out(out),
    m_namemap(new NameMap()) {
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerate", dmgr);
}

TaskGenerate::~TaskGenerate() {

}

bool TaskGenerate::generate() {
    std::string actor = "pss_top__Entry";
    m_out_pub = IOutputUP(new OutputStr());
    m_out_prv = IOutputUP(new OutputStr());

    attach_custom_gen();

    arl::dm::IDataTypeActivitySequenceUP root_activity(
        m_ctxt->mkDataTypeActivitySequence());

    root_activity->addActivity(
        m_ctxt->mkTypeFieldActivity("", 
            m_ctxt->mkDataTypeActivityTraverseType(m_action_t, 0), true), true);

    m_out_prv->println("package %s_prv;", actor.c_str());
    m_out_prv->inc_ind();
    m_out_prv->println("import zsp_sv::*;");

    m_out_pub->println("package %s_pkg;", actor.c_str());
    m_out_pub->inc_ind();
    m_out_pub->println("import %s_prv::*;", actor.c_str());

    // TODO: generate content
    TypeCollectionUP types(TaskBuildTypeCollection(m_dmgr).build(
        m_comp_t,
        m_action_t));
    std::vector<int32_t> sorted = types->sort();

    for (std::vector<int32_t>::const_iterator
        it=sorted.begin();
        it!=sorted.end(); it++) {
        TaskDefineType(this, m_out_prv.get()).generate(
            types->getType(*it));
    }

    std::vector<ActivityInfoUP> activity_info = TaskBuildActivityInfo(
        m_dmgr,
        m_namemap.get()).build(
            0,
            root_activity.get());
    
    DEBUG("activity_info: %d", activity_info.size());

    for (std::vector<ActivityInfoUP>::const_iterator
        it=activity_info.begin();
        it!=activity_info.end(); it++) {

        DEBUG("variants: %d", (*it)->variants().size());

        for (std::vector<ActivityVariantUP>::const_iterator
            v_it=(*it)->variants().begin(); 
            v_it!=(*it)->variants().end(); v_it++) {
            TaskGenerateActivity(
                this, 
                0, 
                m_out_prv.get()
            ).generate(v_it->get());
        }
    }

    m_out_prv->dec_ind();
    m_out_prv->println("endpackage");

    // Define the actor
    m_out_pub->println("class %s extends actor #(comp_t=%s, activity_t=activity_%p);", 
        actor.c_str(),
        getNameMap()->getName(m_comp_t).c_str(),
        root_activity.get());


    m_out_pub->println("endclass");

    m_out_pub->dec_ind();
    m_out_pub->println("endpackage");

    const std::string &s = dynamic_cast<OutputStr *>(m_out_prv.get())->getValue();
    m_out->write(s.c_str(), s.size());

    const std::string &prv = dynamic_cast<OutputStr *>(m_out_pub.get())->getValue();
    m_out->write(prv.c_str(), prv.size());

    return true;
}

void TaskGenerate::attach_custom_gen() {
    DEBUG_ENTER("attach_custom_gen");

    vsc::dm::IDataTypeStruct *addr_handle_t = m_ctxt->findDataTypeStruct(
        "addr_reg_pkg::addr_handle_t");
#ifdef UNDEFINED
    m_name_m->setName(addr_handle_t, "zsp_rt_addr_handle");
    addr_handle_t->setAssociatedData(new TaskGenerateExecModelAddrHandle(getDebugMgr()));
    m_addr_handle_t = addr_handle_t;
#endif

    arl::dm::IDataTypeFunction *f_t;
    f_t = m_ctxt->findDataTypeFunction("addr_reg_pkg::make_handle_from_handle");
#ifdef UNDEFINED
    m_name_m->setName(f_t, "zsp_rt_make_handle_from_handle");
    // f_t->setAssociatedData(
    //     new TaskGenerateExecModelCoreMethodCall(
    //         m_dmgr,
    //         "zsp_rt_make_handle_from_handle",
    //         0,
    //         {"zsp_rt_addr_claim_t *"}));
#endif

    f_t = m_ctxt->findDataTypeFunction("addr_reg_pkg::make_handle_from_claim");
#ifdef UNDEFINED
    m_name_m->setName(f_t, "zsp_rt_make_handle_from_claim");
    f_t->setAssociatedData(
        new TaskGenerateExecModelCoreMethodCall(
            m_dmgr,
            "zsp_rt_make_handle_from_claim",
            0,
            {"zsp_rt_addr_claimspec_t *"}));
#endif

    std::vector<std::string> rw_funcs = {
        "addr_reg_pkg::write64",
        "addr_reg_pkg::write32",
        "addr_reg_pkg::write16",
        "addr_reg_pkg::write8",
        "addr_reg_pkg::read64",
        "addr_reg_pkg::read32",
        "addr_reg_pkg::read16",
        "addr_reg_pkg::read8"
    };
    for (std::vector<std::string>::const_iterator
        it=rw_funcs.begin();
        it!=rw_funcs.end(); it++) {
        f_t = m_ctxt->findDataTypeFunction(*it);
#ifdef UNDEFINED
        f_t->setAssociatedData(new TaskGenerateExecModelMemRwCall(m_dmgr));
#endif
    }

    for (std::vector<vsc::dm::IDataTypeStructUP>::const_iterator
        it=m_ctxt->getDataTypeStructs().begin();
        it!=m_ctxt->getDataTypeStructs().end(); it++) {
        const std::string &name = (*it)->name();

#ifdef UNDEFINED
        if (name.find("::contiguous_addr_space_c") != -1 
            || name.find("::transparent_addr_space_c") != -1) {
//            m_name_m->setName(it->get(), "zsp_rt_addr_space");
        }
#endif
    }

    for (std::vector<arl::dm::IDataTypeFunction *>::const_iterator
        it=m_ctxt->getDataTypeFunctions().begin();
        it!=m_ctxt->getDataTypeFunctions().end(); it++) {
        std::string name = (*it)->name();
        DEBUG("name: %s", name.c_str());
        if (name.find("addr_reg_pkg::") == 0) {
            if (name.find("::addr_handle_t") != -1) {

            } else if (name.find("::contiguous_addr_space_c") != -1 
                || name.find("::transparent_addr_space_c") != -1) {
                std::string rt_name = (name.find("add_region") != -1)?
                    "zsp_rt_addr_space_add_region":
                    "zsp_rt_addr_space_add_nonallocatable_region";

#ifdef UNDEFINED
                (*it)->setAssociatedData(
                    new TaskGenerateExecModelCoreMethodCall(
                        m_dmgr,
                        rt_name,
                        0,
                        {"zsp_rt_addr_space_t *", "zsp_rt_addr_region_t *"}));
#endif
            } else if (name.find("::reg_group_c") != -1) {
                if (name.find("set_handle") != -1) {
#ifdef UNDEFINED
                    (*it)->setAssociatedData(
                        new TaskGenerateExecModelCoreMethodCall(
                            m_dmgr,
                            "zsp_rt_reg_group_set_handle",
                            0,
                            {"void **"}));
#endif
                }
                std::string rt_name = (name.find("add_region") != -1)?
                    "zsp_rt_addr_space_add_region":
                    "zsp_rt_addr_space_add_nonallocatable_region";
            } else if (name.find("::reg_c") != -1) {
                DEBUG("Attach reg-access generator");
#ifdef UNDEFINED
                (*it)->setAssociatedData(
                    new TaskGenerateExecModelRegRwCall(m_dmgr));
#endif
            }
        } else if (name.find("std_pkg::") == 0) {
            if (name.find("urandom_range") != -1) {
#ifdef UNDEFINED
                (*it)->setAssociatedData(
                    new TaskGenerateExecModelCoreMethodCall(
                        m_dmgr,
                        "zsp_rt_urandom_range",
                        -1, {}));
#endif
            } else if (name.find("urandom") != -1) {
#ifdef UNDEFINED
                (*it)->setAssociatedData(
                    new TaskGenerateExecModelCoreMethodCall(
                        m_dmgr,
                        "zsp_rt_urandom",
                        -1, {}));
#endif
            } else if (name.find("print") != -1) {
                DEBUG("Attach custom generator to 'print'");
                (*it)->setAssociatedData(
                    new CustomGenPrintCall(m_dmgr)
                );
            }
        }
    }

    DEBUG_LEAVE("attach_custom_gen");
}


dmgr::IDebug *TaskGenerate::m_dbg = 0;

}
}
}
}
