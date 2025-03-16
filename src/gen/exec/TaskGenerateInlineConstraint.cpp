/*
 * TaskGenerateInlineConstraint.cpp
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
#include "TaskGenerateActorPkgPrv.h"
#include "TaskGenerateInlineConstraint.h"
#include "TaskGenerateConstraint.h"
#include "GenRefExprTraversal.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateInlineConstraint::TaskGenerateInlineConstraint(
        TaskGenerateActorPkgPrv *gen,
        IGenRefExpr             *genref,
        IOutput                 *out) : m_gen(gen), m_genref(genref), m_out(out) {
    DEBUG_INIT("Zsp::gen::exec::TaskGenerateInlineConstraint", gen->getDebugMgr());
}

TaskGenerateInlineConstraint::~TaskGenerateInlineConstraint() {

}

void TaskGenerateInlineConstraint::generate(
        vsc::dm::ITypeConstraint    *c,
        const std::string           &name,
        vsc::dm::IDataTypeStruct    *action_t,
        vsc::dm::IDataTypeStruct    *parent_t) {
    DEBUG_ENTER("generate action_t=%s parent_t=%s", 
        m_gen->getNameMap()->getName(action_t).c_str(), 
        m_gen->getNameMap()->getName(parent_t).c_str());
    m_out->println("class %s extends action_constraint_c #(%s);", 
        name.c_str(),
        m_gen->getNameMap()->getName(action_t).c_str());
    m_out->inc_ind();
    m_out->println("%s __parent;", 
        m_gen->getNameMap()->getName(parent_t).c_str());
    m_out->println("");
    m_out->println("function new(%s parent);", 
        m_gen->getNameMap()->getName(parent_t).c_str());
    m_out->inc_ind();
    m_out->println("__parent = parent;");
    m_out->dec_ind();
    m_out->println("endfunction");
    m_out->println("");
    m_out->println("static function %s mk(%s parent);", 
        name.c_str(), 
        m_gen->getNameMap()->getName(parent_t).c_str());
    m_out->inc_ind();
    m_out->println("%s ret = new(parent);", name.c_str());
    m_out->println("return ret;");
    m_out->dec_ind();
    m_out->println("endfunction");
    m_out->println("");
    m_out->println("constraint __inline {");
    m_out->inc_ind();
    GenRefExprTraversal genref(m_gen, action_t, "__self", false, parent_t);
    TaskGenerateConstraint(m_gen, &genref, m_out).generate(c);
    m_out->dec_ind();
    m_out->println("}");
    m_out->println("");
    m_out->dec_ind();
    m_out->println("endclass");
    m_out->println("");

    DEBUG_LEAVE("generate");
}

dmgr::IDebug *TaskGenerateInlineConstraint::m_dbg = 0;

}
}
}
}

