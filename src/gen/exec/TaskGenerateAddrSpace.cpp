/*
 * TaskGenerateAddrSpace.cpp
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
#include "TaskGenerateAddrSpace.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateAddrSpace::TaskGenerateAddrSpace(
    TaskGenerate        *gen,
    IOutput             *out) : m_gen(gen), m_out(out) {
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateAddrSpace", gen->getDebugMgr());
}

TaskGenerateAddrSpace::~TaskGenerateAddrSpace() {

}

void TaskGenerateAddrSpace::generate(vsc::dm::IDataTypeStruct *t) {
    m_out->println("class %s extends addr_space_c;", m_gen->getNameMap()->getName(t).c_str());
    m_out->inc_ind();
    m_out->println("function new(string name, component_ctor_ctxt ctxt, component_c parent);");
    m_out->inc_ind();
    m_out->println("super.new(name, ctxt, parent);");
    m_out->dec_ind();
    m_out->println("endfunction");
    m_out->dec_ind();
    m_out->println("endclass");
}

dmgr::IDebug *TaskGenerateAddrSpace::m_dbg = 0;

}
}
}
}
