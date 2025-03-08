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
#include "CustomGenAddrHandle.h"
#include "CustomGenAddrRegion.h"
#include "CustomGenAddrRegionTransparent.h"
#include "CustomGenCoreMethodCall.h"
#include "CustomGenCoreType.h"
#include "CustomGenExecFunc.h"
#include "CustomGenImportCall.h"
#include "CustomGenMemRwCall.h"
#include "CustomGenMessageCall.h"
#include "CustomGenPrintCall.h"
#include "CustomGenRegAccessCall.h"
#include "CustomGenRegGetHandle.h"
#include "GenRefExprExecModel.h"
#include "TaskDefineType.h"
#include "TaskGenerate.h"
#include "TaskGenerateActivity.h"
#include "TaskGenerateImportApi.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerate::TaskGenerate(
    dmgr::IDebugMgr                 *dmgr,
    arl::dm::IContext               *ctxt,
    arl::eval::IFactory             *eval_f,
    std::ostream                    *out) : 
    m_dbg(0), m_dmgr(dmgr), m_ctxt(ctxt),
    m_eval_f(eval_f), m_out(out), m_namemap(new NameMap()) {
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerate", dmgr);
}

TaskGenerate::~TaskGenerate() {

}

void TaskGenerate::attach_custom_gen() {
    DEBUG_ENTER("attach_custom_gen");

    vsc::dm::IDataTypeStruct *addr_handle_t = m_ctxt->findDataTypeStruct(
        "addr_reg_pkg::addr_handle_t");
    m_namemap->setName(addr_handle_t, "addr_handle_t");
    addr_handle_t->setAssociatedData(new CustomGenAddrHandle(getDebugMgr()));
#ifdef UNDEFINED
    m_addr_handle_t = addr_handle_t;
#endif

    arl::dm::IDataTypeFunction *f_t;
    f_t = m_ctxt->findDataTypeFunction("addr_reg_pkg::make_handle_from_handle");
    f_t->setAssociatedData(new CustomGenCoreMethodCall(m_dmgr));
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
    f_t->setAssociatedData(new CustomGenCoreMethodCall(m_dmgr));

    f_t = m_ctxt->findDataTypeFunction("addr_reg_pkg::addr_value");
    f_t->setAssociatedData(new CustomGenCoreMethodCall(m_dmgr));
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
        f_t->setAssociatedData(new CustomGenMemRwCall(m_dmgr));
    }

    std::vector<std::string> exec_funcs = {
        "addr_reg_pkg::addr_value"
    };
    for (std::vector<std::string>::const_iterator
        it=exec_funcs.begin();
        it!=exec_funcs.end(); it++) {
        f_t = m_ctxt->findDataTypeFunction(*it);
        f_t->setAssociatedData(new CustomGenExecFunc(m_dmgr));
    }

    std::vector<vsc::dm::IDataTypeStruct *> addr_region_transparent_bases;
    std::vector<vsc::dm::IDataTypeStruct *> addr_region_base_bases;

    for (std::vector<vsc::dm::IDataTypeStructUP>::const_iterator
        it=m_ctxt->getDataTypeStructs().begin();
        it!=m_ctxt->getDataTypeStructs().end(); it++) {
        const std::string &name = (*it)->name();
        if (name.find("::transparent_addr_region_s") != -1) {
            addr_region_transparent_bases.push_back(it->get());
        } else if (name.find("::addr_region_base_s") != -1) {
            addr_region_base_bases.push_back(it->get());
        }
    }


    for (std::vector<vsc::dm::IDataTypeStructUP>::const_iterator
        it=m_ctxt->getDataTypeStructs().begin();
        it!=m_ctxt->getDataTypeStructs().end(); it++) {
        const std::string &name = (*it)->name();
        const std::string sname = ((*it)->getSuper())?(*it)->getSuper()->name():"";

        DEBUG("type name: %s sname: %s", name.c_str(), sname.c_str());

        if (isInstance(it->get(), addr_region_transparent_bases)) {
            DEBUG("Is derived from transparent_addr_region");
            (*it)->setAssociatedData(new CustomGenAddrRegionTransparent(getDebugMgr()));
        } else if (isInstance(it->get(), addr_region_base_bases)) {
            DEBUG("Is derived from addr_region_base");
            std::string leaf = name.substr(name.find("::")+2);
            m_namemap->setName(it->get(), leaf);

            if (it->get()->name().find("addr_reg_pkg::") == 0) {
                (*it)->setAssociatedData(new CustomGenCoreType(m_dmgr));
            } else {
                (*it)->setAssociatedData(new CustomGenAddrRegion(getDebugMgr()));
            }
        } else if (isInstance(it->get(), addr_region_base_bases, false)
            || isInstance(it->get(), addr_region_transparent_bases, false)) {
            // Check if this *is* addr_region_base_s
            DEBUG("Is addr_region_base_s or transparent_addr_region_s");
            std::string leaf = name.substr(name.find("::")+2);
            m_namemap->setName(it->get(), leaf);
        } else if (name.find("addr_reg_pkg::") == 0
            || name.find("executor_pkg::") == 0
            || name.find("std_pkg::") == 0) {
            DEBUG("Add for type %s", name.c_str());
            std::string leaf = name.substr(name.find("::")+2);
            (*it)->setAssociatedData(new CustomGenCoreType(m_dmgr));
            m_namemap->setName(it->get(), leaf);
        }

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
            } else if (name.find("::reg_c") != -1) {
                DEBUG("Reg method: %s", name.c_str());
                if (name.find("::get_handle") != -1) {
                    (*it)->setAssociatedData(new CustomGenRegGetHandle(m_dmgr));
                } else if (name.find("::write") != -1 || name.find("::read") != -1) {
                    DEBUG("Attach reg read-write custom generator");
                    (*it)->setAssociatedData(new CustomGenRegAccessCall(m_dmgr));
                }
                // TODO: handle get_handle()
            } else if (name.find("::reg_group_c") != -1) {
                (*it)->setFlags(arl::dm::DataTypeFunctionFlags::Solve);
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
            } else if (name.find("read") != -1 || name.find("write") != -1) {
                DEBUG("Marking function %s as target", name.c_str());
                (*it)->setFlags(arl::dm::DataTypeFunctionFlags::Target);
            }
        } else if (name.find("executor_pkg::") == 0) {
            (*it)->setAssociatedData(
                new CustomGenCoreMethodCall(m_dmgr));
                /*
                    "zsp_rt_urandom_range",
                    -1, {}));
                 */
        } else if (name.find("std_pkg::") == 0) {
            (*it)->setAssociatedData(new CustomGenCoreMethodCall(m_dmgr));
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
            } else if (name.find("message") != -1) {
                DEBUG("Attach custom generator to 'message'");
                (*it)->setAssociatedData(
                    new CustomGenMessageCall(m_dmgr)
                );
            } else if (name.find("print") != -1) {
                DEBUG("Attach custom generator to 'print'");
                (*it)->setAssociatedData(
                    new CustomGenPrintCall(m_dmgr)
                );
            }
        } else {
            // This is an import function
            if ((*it)->getImportSpecs().size()) {
                DEBUG("TODO: Attach custom generator to %s", name.c_str());
                (*it)->setAssociatedData(
                    new CustomGenImportCall(m_dmgr));
            }
        }
    }

    DEBUG_LEAVE("attach_custom_gen");
}

bool TaskGenerate::isInstance(
        vsc::dm::IDataTypeStruct                        *t, 
        const std::vector<vsc::dm::IDataTypeStruct *>   &bases,
        bool                                            excl_self) {
    bool ret = false;
    int32_t depth = 0;

//    DEBUG_ENTER("isInstance");

    while (t && !ret) {
//        DEBUG("t=%s", t->name().c_str());
        if (depth || !excl_self) {
            for (std::vector<vsc::dm::IDataTypeStruct *>::const_iterator
                it=bases.begin();
                it!=bases.end() && !ret; it++) {
                if (t == *it) {
 //                   DEBUG("is-a %s", (*it)->name().c_str());
                    ret = true;
                    break;
                }
            }
        }

        t = t->getSuper();

        depth++;
    }

//    DEBUG_LEAVE("isInstance %d", ret);

    return ret;
}


}
}
}
}
