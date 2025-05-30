/*
 * CustomGenAddrRegionTransparent.cpp
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
#include "CustomGenAddrRegionTransparent.h"
#include "TaskGenerateAddrRegionTransparent.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


CustomGenAddrRegionTransparent::CustomGenAddrRegionTransparent(
    dmgr::IDebugMgr *dmgr) : CustomGenBase(dmgr) {
    m_dbg = 0;
    DEBUG_INIT("zsp::sv::gen::exec::CustomGenAddrRegionTransparent", dmgr);
}

CustomGenAddrRegionTransparent::~CustomGenAddrRegionTransparent() {

}

void CustomGenAddrRegionTransparent::genDefinition(
        TaskGenerate                        *gen,
        IOutput                             *out,
        vsc::dm::IDataType                  *type) {
    DEBUG_ENTER("genDefinition");
    TaskGenerateAddrRegionTransparent(gen, out).generate(dynamic_cast<vsc::dm::IDataTypeStruct *>(type));
    DEBUG_LEAVE("genDefinition");
}


}
}
}
}
