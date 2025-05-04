/*
 * TaskGenerateStructMethods.cpp
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
#include "ICustomGen.h"
#include "TaskGenerate.h"
#include "GenRefExprExecModel.h"
#include "TaskGenerateFunction.h"
#include "TaskGenerateStructMethods.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateStructMethods::TaskGenerateStructMethods(
        TaskGenerate             *gen,
        IOutput                  *out) : m_gen(gen), m_out(out) {
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateStructMethods", gen->getDebugMgr());
}

TaskGenerateStructMethods::~TaskGenerateStructMethods() {

}

void TaskGenerateStructMethods::generate(vsc::dm::IDataTypeStruct *t) {
    DEBUG_ENTER("generate %s", t->name().c_str());
    t->accept(this);
    DEBUG_LEAVE("generate %s", t->name().c_str());
}

void TaskGenerateStructMethods::visitDataTypeArlStruct(arl::dm::IDataTypeArlStruct *t) {
    DEBUG_ENTER("visitDataTypeArlStruct %s", t->name().c_str());

    GenRefExprExecModel genref(m_gen, t, "this", false, "", false);
    for (std::vector<arl::dm::IDataTypeFunctionUP>::const_iterator
        it=t->getFunctions().begin();
        it!=t->getFunctions().end(); it++) {
        ICustomGen *custom_gen = 
            dynamic_cast<ICustomGen *>((*it)->getAssociatedData());

        if (custom_gen) {
            custom_gen->genFunctionDefinition(m_gen, m_out, &genref, it->get());
        } else {
            // Generate the function
            TaskGenerateFunction(m_gen, &genref, m_out).generate(it->get(), true);
        }
    }

    DEBUG_LEAVE("visitDataTypeArlStruct %s", t->name().c_str());
}

void TaskGenerateStructMethods::visitDataTypeComponent(arl::dm::IDataTypeComponent *t) {
    DEBUG_ENTER("visitDataTypeComponent %s", t->name().c_str());

    GenRefExprExecModel genref(m_gen, t, "this", false, "", false);
    for (std::vector<arl::dm::IDataTypeFunctionUP>::const_iterator
        it=t->getFunctions().begin();
        it!=t->getFunctions().end(); it++) {
        if (!(*it)->hasFlags(arl::dm::DataTypeFunctionFlags::Core)) {
            TaskGenerateFunction(m_gen, &genref, m_out).generate(it->get(), true);
        }
    }

    DEBUG_LEAVE("visitDataTypeComponent");
}


dmgr::IDebug *TaskGenerateStructMethods::m_dbg = 0;

}
}
}
}
