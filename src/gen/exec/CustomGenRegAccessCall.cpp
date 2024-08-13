/*
 * CustomGenRegAccessCall.cpp
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
#include "CustomGenRegAccessCall.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


CustomGenRegAccessCall::CustomGenRegAccessCall(dmgr::IDebugMgr *dmgr) :
    CustomGenBase(dmgr) {
    m_dbg = 0;
    DEBUG_INIT("zsp::sv::gen::exec::CustomGenRegAccessCall", dmgr);

}

CustomGenRegAccessCall::~CustomGenRegAccessCall() {

}

void CustomGenRegAccessCall::genExprMethodCallContext(
        TaskGenerate                        *gen,
        IOutput                             *out,
        IGenRefExpr                         *refgen,
        arl::dm::ITypeExprMethodCallContext *call) {
    DEBUG_ENTER("genExprMethodCallCallContext %s", call->getTarget()->name().c_str());
    const std::string &name = call->getTarget()->name();
    int32_t width = 32; // TODO: calculate from context
    int64_t offset = 0; // TODO: caclucate from context

    if (name.find("::write_val") != -1) {
    } else if (name.find("::read_val") != -1) {
    } else if (name.find("::write") != -1) {
    } else if (name.find("::read") != -1) {

    }

    DEBUG_LEAVE("genExprMethodCallCallContext");
}

}
}
}
}
