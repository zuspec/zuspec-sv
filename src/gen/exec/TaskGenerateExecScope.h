/**
 * TaskGenerateExecScope.h
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
#include "gen/OutputExecScope.h"
#include "IGenRefExpr.h"

namespace zsp {
namespace sv {
namespace gen {
namespace exec {

class TaskGenerate;

class TaskGenerateExecScope :
    public virtual arl::dm::VisitorBase {
public:
    TaskGenerateExecScope(
        TaskGenerate        *gen,
        IGenRefExpr         *genref,
        IOutput             *out
    );

    virtual ~TaskGenerateExecScope();

    virtual void generate(
        arl::dm::ITypeProcStmtScope *scope,
        bool                        newscope,
        bool                        istask=false);

    virtual void generate(
        arl::dm::ITypeProcStmt      *stmt,
        bool                        istask=false);

    virtual void visitTypeProcStmtAssign(arl::dm::ITypeProcStmtAssign *s) override;

	virtual void visitTypeProcStmtExpr(arl::dm::ITypeProcStmtExpr *s) override;

    virtual void visitTypeProcStmtForeach(arl::dm::ITypeProcStmtForeach *s) override;

    virtual void visitTypeProcStmtIfClause(arl::dm::ITypeProcStmtIfClause *s) override;

    virtual void visitTypeProcStmtIfElse(arl::dm::ITypeProcStmtIfElse *s) override;

    virtual void visitTypeProcStmtRepeat(arl::dm::ITypeProcStmtRepeat *s) override;

    virtual void visitTypeProcStmtRepeatWhile(arl::dm::ITypeProcStmtRepeatWhile *s) override;

    virtual void visitTypeProcStmtReturn(arl::dm::ITypeProcStmtReturn *s) override;

    virtual void visitTypeProcStmtScope(arl::dm::ITypeProcStmtScope *s) override;

    virtual void visitTypeProcStmtVarDecl(arl::dm::ITypeProcStmtVarDecl *t) override;

	virtual void visitTypeProcStmtWhile(arl::dm::ITypeProcStmtWhile *s) override;

	virtual void visitTypeProcStmtYield(arl::dm::ITypeProcStmtYield *s) override;

protected:
    dmgr::IDebug                    *m_dbg;
    TaskGenerate                    *m_gen;
    IGenRefExpr                     *m_genref;
    IOutput                         *m_out_top;
    IOutput                         *m_out;
    std::vector<OutputExecScope *>  m_exec_s;
    bool                            m_istask;
    int32_t                         m_depth;

};

}
}
}
}


