/*
 * Factory.cpp
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
#include "Factory.h"
#include "gen/exec/TaskGenerate.h"


namespace zsp {
namespace sv {


Factory::Factory() : m_dmgr(0) {

}

Factory::~Factory() {

}

void Factory::init(dmgr::IDebugMgr *dmgr) {
    m_dmgr = dmgr;
}


gen::ITaskGenerate *Factory::mkGenerateExecActor(
        arl::dm::IContext               *ctxt,
        arl::dm::IDataTypeComponent     *comp_t,
        arl::dm::IDataTypeAction        *action_t,
        std::ostream                    *out) {
    return new gen::exec::TaskGenerate(
        m_dmgr,
        ctxt,
        comp_t,
        action_t,
        out);
}

Factory *Factory::inst() {
    if (!m_inst) {
        m_inst = FactoryUP(new Factory());
    }
    return m_inst.get();
}

FactoryUP Factory::m_inst;


}
}

extern "C" zsp::sv::IFactory *zsp_sv_getFactory() {
    return zsp::sv::Factory::inst();
}
