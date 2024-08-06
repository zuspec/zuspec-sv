/**
 * TaskGenerate.h
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
#pragma once
#include <iostream>
#include "zsp/arl/dm/IContext.h"
#include "zsp/sv/gen/ITaskGenerate.h"
#include "gen/INameMap.h"
#include "gen/IOutput.h"
#include "gen/TypeCollection.h"

namespace zsp {
namespace sv {
namespace gen {
namespace exec {



class TaskGenerate : public virtual gen::ITaskGenerate {
public:
    TaskGenerate(
        dmgr::IDebugMgr                 *dmgr,
        arl::dm::IContext               *ctxt,
        arl::dm::IDataTypeComponent     *comp_t,
        arl::dm::IDataTypeAction        *action_t,
        std::ostream                    *out);

    virtual ~TaskGenerate();

    virtual bool generate();

    dmgr::IDebugMgr *getDebugMgr() { return m_dmgr; }

    INameMap *getNameMap() { return m_namemap.get(); }

    const std::string &getActorName() { return m_actor_name; }

protected:

    void attach_custom_gen();

protected:
    static dmgr::IDebug             *m_dbg;
    dmgr::IDebugMgr                 *m_dmgr;
    arl::dm::IContext               *m_ctxt;
    arl::dm::IDataTypeComponent     *m_comp_t;
    arl::dm::IDataTypeAction        *m_action_t;
    std::ostream                    *m_out;
    IOutputUP                       m_out_pub;
    IOutputUP                       m_out_prv;
    INameMapUP                      m_namemap;
    std::string                     m_actor_name;



};

}
}
}
}


