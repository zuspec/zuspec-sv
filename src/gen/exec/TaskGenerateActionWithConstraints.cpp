/**
 * TaskGenerateActionWithConstraints.cpp
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
#include "TaskGenerateActionWithConstraints.h"
#include "TaskGenerate.h"
#include "TaskGenerateConstraint.h"

namespace zsp {
namespace sv {
namespace gen {
namespace exec {

TaskGenerateActionWithConstraints::TaskGenerateActionWithConstraints(
    TaskGenerate         *gen,
    IGenRefExpr         *genref,
    IOutput             *out) : 
    TaskGenerateStructConstraints(gen, genref, out),
    m_dbg(0), m_gen(gen), m_genref(genref), m_out(out), 
    m_traversal_count(0), m_action_t(0) {
    m_dbg = gen->getDebugMgr()->findDebug("TaskGenerateActionWithConstraints");
}

TaskGenerateActionWithConstraints::~TaskGenerateActionWithConstraints() {

}

void TaskGenerateActionWithConstraints::generate(arl::dm::IDataTypeAction *t) {
    m_action_t = t;
    m_traversal_count = 0;

    // Visit activity nodes to find traversals with 'with' clauses
    for (auto it=t->activities().begin(); it!=t->activities().end(); it++) {
        (*it)->accept(m_this);
    }
}

void TaskGenerateActionWithConstraints::visitDataTypeActivityTraverse(
    arl::dm::IDataTypeActivityTraverse *t) {
    if (t->getWithC()) {
        std::string action_type = m_action_t->name();
        generateInlineWithClass(
            action_type,
            m_traversal_count++,
            t->getWithC());
    }
}

void TaskGenerateActionWithConstraints::visitDataTypeActivityTraverseType(
    arl::dm::IDataTypeActivityTraverseType *t) {
    if (t->getWithC()) {
        std::string action_type = m_action_t->name();
        generateInlineWithClass(
            action_type,
            m_traversal_count++,
            t->getWithC());
    }
}

void TaskGenerateActionWithConstraints::generateInlineWithClass(
    const std::string           &action_type,
    int32_t                     traversal_id,
    vsc::dm::ITypeConstraint *constraints) {
    
    m_out->println("class action_%s_inline_with_%d;", 
        action_type.c_str(), traversal_id);
    m_out->inc_ind();
    
    // Use TaskGenerateConstraint to generate the inline constraint
    if (constraints) {
        TaskGenerateConstraint(m_gen, m_genref, m_out).generate(constraints);
    }
    
    m_out->dec_ind();
    m_out->println("endclass");
    m_out->println("");
}

}
}
}
}
