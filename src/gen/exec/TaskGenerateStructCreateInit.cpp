/*
 * TaskGenerateStructCreateInit.cpp
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
#include "TaskGenerateStructCreateInit.h"
#include "TaskGenerateDataType.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateStructCreateInit::TaskGenerateStructCreateInit(
        TaskGenerate            *gen,
        IOutput                 *out) : m_gen(gen), m_out(out) {
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateStructCreateInit", gen->getDebugMgr());
}

TaskGenerateStructCreateInit::~TaskGenerateStructCreateInit() {

}

void TaskGenerateStructCreateInit::generate(vsc::dm::IDataTypeStruct *t) {
    if (t->getFields().size()) {
        m_out->println("static function %s create_init(",
            m_gen->getNameMap()->getName(t).c_str());
        m_out->inc_ind();
        m_out->inc_ind();
        for (std::vector<vsc::dm::ITypeFieldUP>::const_iterator
            it=t->getFields().begin();
            it!=t->getFields().end(); it++) {
            m_out->indent();
            TaskGenerateDataType(m_gen, m_out).generate((*it)->getDataType());
            m_out->write(" %s%s",
                (*it)->name().c_str(),
                ((it+1) != t->getFields().end())?",\n":");\n");
        }
        m_out->dec_ind();
    } else {
        m_out->println("static function %s create_init();",
            m_gen->getNameMap()->getName(t).c_str());
    }

    m_out->println("%s ret = new();",
        m_gen->getNameMap()->getName(t).c_str());
    for (std::vector<vsc::dm::ITypeFieldUP>::const_iterator
        it=t->getFields().begin();
        it!=t->getFields().end(); it++) {
        m_out->println("ret.%s = %s;", 
            (*it)->name().c_str(),
            (*it)->name().c_str());
    }
    m_out->println("return ret;");
    m_out->dec_ind();
    m_out->println("endfunction");
}

dmgr::IDebug *TaskGenerateStructCreateInit::m_dbg = 0;

}
}
}
}
