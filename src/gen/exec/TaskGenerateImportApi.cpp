/*
 * TaskGenerateImportApi.cpp
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
#include "TaskGenerateImportApi.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateImportApi::TaskGenerateImportApi(
        TaskGenerate        *gen,
        IOutput             *out) : m_gen(gen), m_out(out) {
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateImportApi", gen->getDebugMgr());
}

TaskGenerateImportApi::~TaskGenerateImportApi() {

}

void TaskGenerateImportApi::generate(const std::vector<arl::dm::IDataTypeFunction *> &funcs) {
    m_out->println("class import_api extends backend_api;");
    m_out->inc_ind();
    for (std::vector<arl::dm::IDataTypeFunction *>::const_iterator
        it=funcs.begin();
        it!=funcs.end(); it++) {
        (*it)->accept(m_this);
    }
    m_out->dec_ind();
    m_out->println("endclass");
}

}
}
}
}
