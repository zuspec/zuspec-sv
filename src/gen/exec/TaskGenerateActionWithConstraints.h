/**
 * TaskGenerateActionWithConstraints.h
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
#pragma once
#include "gen/exec/TaskGenerateStructConstraints.h"

namespace zsp {
namespace sv {
namespace gen {
namespace exec {

class TaskGenerateActionWithConstraints : 
    public virtual TaskGenerateStructConstraints {
public:
    TaskGenerateActionWithConstraints(
        TaskGenerate        *gen,
        IGenRefExpr         *genref,
        IOutput             *out);

    virtual ~TaskGenerateActionWithConstraints();

    virtual void generate(arl::dm::IDataTypeAction *t);

    virtual void visitDataTypeActivityTraverse(arl::dm::IDataTypeActivityTraverse *t) override;

    virtual void visitDataTypeActivityTraverseType(arl::dm::IDataTypeActivityTraverseType *t) override;

protected:
    void generateInlineWithClass(
        const std::string           &action_type,
        int32_t                     traversal_id,
        vsc::dm::ITypeConstraint *constraints);

private:
    dmgr::IDebug                *m_dbg;
    TaskGenerate                *m_gen;
    IGenRefExpr                 *m_genref;
    IOutput                     *m_out;
    int32_t                     m_traversal_count;
    arl::dm::IDataTypeAction   *m_action_t;
};

}
}
}
}
