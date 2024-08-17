/*
 * TaskGenerateAddrRegion.cpp
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
#include "TaskGenerateAddrRegion.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateAddrRegion::TaskGenerateAddrRegion(
    TaskGenerate        *gen,
    IOutput             *out) : TaskGenerateStruct(gen, out) {
    m_dbg = 0;
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateAddrRegion", gen->getDebugMgr());
}

TaskGenerateAddrRegion::~TaskGenerateAddrRegion() {

}

void TaskGenerateAddrRegion::generate_head(vsc::dm::IDataTypeStruct *t) {
    m_out->println("class %s extends object #(executor_base_c) implements addr_region_base;",
        m_gen->getNameMap()->getName(t).c_str());
    m_out->inc_ind();
}

}
}
}
}
