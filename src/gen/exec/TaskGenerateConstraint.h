/**
 * TaskGenerateConstraint.h
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
#include "dmgr/IDebugMgr.h"
#include "zsp/arl/dm/impl/VisitorBase.h"
#include "gen/IOutput.h"
#include "IGenRefExpr.h"

namespace zsp {
namespace sv {
namespace gen {
namespace exec {

class TaskGenerate;

class TaskGenerateConstraint : 
    public virtual arl::dm::VisitorBase {
public:
    TaskGenerateConstraint(
        TaskGenerate        *gen,
        IGenRefExpr         *genref,
        IOutput             *out);

    virtual ~TaskGenerateConstraint();

    void generate(
        vsc::dm::ITypeConstraint    *c);

	virtual void visitTypeConstraintBlock(vsc::dm::ITypeConstraintBlock *c) override;

	virtual void visitTypeConstraintExpr(vsc::dm::ITypeConstraintExpr *c) override;

	virtual void visitTypeConstraintForeach(vsc::dm::ITypeConstraintForeach *c) override;

	virtual void visitTypeConstraintIfElse(vsc::dm::ITypeConstraintIfElse *c) override;

	virtual void visitTypeConstraintImplies(vsc::dm::ITypeConstraintImplies *c) override;

	virtual void visitTypeConstraintSoft(vsc::dm::ITypeConstraintSoft *c) override;

	virtual void visitTypeConstraintUnique(vsc::dm::ITypeConstraintUnique *c) override;

private:
    static dmgr::IDebug         *m_dbg;
    TaskGenerate                *m_gen;
    IGenRefExpr                 *m_genref;
    IOutput                     *m_out;

};

}
}
}
}


