/*
 * TaskGenerateStructAssign.cpp
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
#include "TaskGenerateStructAssign.h"
#include "TaskIsRefCountField.h"
#include "TaskIsValueAggregate.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateStructAssign::TaskGenerateStructAssign(
        TaskGenerate        *gen,
        IOutput             *out) : m_dbg(0), m_gen(gen), m_out(out) {
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateStructAssign", gen->getDebugMgr());
}

TaskGenerateStructAssign::~TaskGenerateStructAssign() {

}

void TaskGenerateStructAssign::generate(vsc::dm::IDataTypeStruct *t) {
    m_out->println("virtual function void __assign__(%s rhs);",
        m_gen->getNameMap()->getName(t).c_str());
    m_out->inc_ind();
    for (std::vector<vsc::dm::ITypeFieldUP>::const_iterator
        it=t->getFields().begin();
        it!=t->getFields().end(); it++) {
        if (TaskIsRefCountField().check(it->get())) {
            m_out->println("%s.set(rhs.%s);", 
                (*it)->name().c_str(),
                (*it)->name().c_str());
        } else if (TaskIsValueAggregate().check((*it)->getDataType())) {
            m_out->println("%s.__assign__(rhs.%s);",
                (*it)->name().c_str(),
                (*it)->name().c_str());
        } else {
            m_out->println("%s = rhs.%s;",
                (*it)->name().c_str(),
                (*it)->name().c_str());
        }
    }
    m_out->dec_ind();
    m_out->println("endfunction");
};

}
}
}
}
