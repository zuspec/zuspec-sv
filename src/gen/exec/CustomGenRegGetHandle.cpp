/*
 * CustomGenRegGetHandle.cpp
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
#include "CustomGenRegGetHandle.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


CustomGenRegGetHandle::CustomGenRegGetHandle(dmgr::IDebugMgr *dmgr) :
    CustomGenBase(dmgr) {
    m_dbg = 0;
    DEBUG_INIT("zsp::sv::gen::exec::CustomGenRegGetHandle", dmgr);
}

CustomGenRegGetHandle::~CustomGenRegGetHandle() {

}

void CustomGenRegGetHandle::genExprMethodCallContext(
        TaskGenerate                        *gen,
        IOutput                             *out,
        IGenRefExpr                         *refgen,
        arl::dm::ITypeExprMethodCallContext *call) {
    DEBUG_ENTER("genExprMethodCallContext");
    out->write("make_handle_from_handle(exec_b.get_actor(), null, ");
    out->write(")");
    DEBUG_LEAVE("genExprMethodCallContext");
}

}
}
}
}
