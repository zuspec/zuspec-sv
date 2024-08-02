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
#include "gen/NameMap.h"
#include "gen/OutputStr.h"
#include "gen/TaskBuildTypeCollection.h"
#include "TaskDefineType.h"
#include "TaskGenerate.h"


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


    m_out_prv->dec_ind();
    m_out_prv->println("endpackage");

    // Define the actor
    m_out_pub->println("class %s extends actor #(comp_t=%s, action_t=%s);", 
        actor.c_str(),
        getNameMap()->getName(m_comp_t).c_str(),
        getNameMap()->getName(m_action_t).c_str());

    m_out_pub->println("endclass");

    m_out_pub->dec_ind();
    m_out_pub->println("endpackage");

    const std::string &s = dynamic_cast<OutputStr *>(m_out_prv.get())->getValue();
    m_out->write(s.c_str(), s.size());

    const std::string &prv = dynamic_cast<OutputStr *>(m_out_pub.get())->getValue();
    m_out->write(prv.c_str(), prv.size());

    return true;
}

dmgr::IDebug *TaskGenerate::m_dbg = 0;

}
}
}
}
