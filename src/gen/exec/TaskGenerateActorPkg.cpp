/*
 * TaskGenerateActorPkg.cpp
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
#include "gen/Output.h"
#include "TaskGenerateActorPkg.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateActorPkg::TaskGenerateActorPkg(
        dmgr::IDebugMgr                 *dmgr,
        arl::dm::IContext               *ctxt,
        arl::eval::IFactory             *eval_f,
        arl::dm::IDataTypeComponent     *comp_t,
        arl::dm::IDataTypeAction        *action_t,
        std::ostream                    *out) : 
        TaskGenerate(dmgr, ctxt, eval_f, out),
        m_comp_t(comp_t), m_action_t(action_t) {
    m_dbg = 0;
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateActorPkg", dmgr);
}

TaskGenerateActorPkg::~TaskGenerateActorPkg() {

}

bool TaskGenerateActorPkg::generate() {
    std::string actor = "pss_top__Entry";
    DEBUG_ENTER("generate");
    OutputUP out(new Output(m_out, false));

    out->println("package %s_pkg;", actor.c_str());
    out->inc_ind();
    out->println("import zsp_sv::*;");
    out->println("import %s_prv::*;", actor.c_str());

    out->println("typedef %s_prv::%s_actor %s;", 
        actor.c_str(), 
        actor.c_str(),
        actor.c_str());

    out->dec_ind();
    out->println("endpackage");

    out->close();
    DEBUG_LEAVE("generate");
    return true;
}

}
}
}
}
