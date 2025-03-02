/**
 * TaskRewriteTargetRvCalls.h
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
#include <vector>
#include "dmgr/IDebugMgr.h"
#include "zsp/arl/dm/impl/CopyVisitor.h"
#include "zsp/arl/dm/impl/VisitorBase.h"

namespace zsp {
namespace sv {
namespace gen {
namespace exec {



class TaskRewriteTargetRvCalls :
    public virtual arl::dm::VisitorBase {
public:
    TaskRewriteTargetRvCalls(
        dmgr::IDebugMgr         *dmgr,
        arl::dm::IContext       *ctxt);

    virtual ~TaskRewriteTargetRvCalls();

    void rewrite(arl::dm::ITypeProcStmtScope *scope);

    virtual void visitTypeProcStmtAssign(arl::dm::ITypeProcStmtAssign *s) override;

    virtual void visitTypeProcStmtReturn(arl::dm::ITypeProcStmtReturn *s) override;

    virtual void visitTypeProcStmtScope(arl::dm::ITypeProcStmtScope *s) override;

	virtual void visitTypeExprArrIndex(vsc::dm::ITypeExprArrIndex *e) override;

	virtual void visitTypeExprBin(vsc::dm::ITypeExprBin *e) override;

	virtual void visitTypeExprFieldRef(vsc::dm::ITypeExprFieldRef *e) override;

	virtual void visitTypeExprRange(vsc::dm::ITypeExprRange *e) override;

	virtual void visitTypeExprRangelist(vsc::dm::ITypeExprRangelist *e) override;

	virtual void visitTypeExprRef(vsc::dm::ITypeExprRef *e) override;

	virtual void visitTypeExprUnary(vsc::dm::ITypeExprUnary *e) override;

    virtual void visitTypeExprMethodCallContext(arl::dm::ITypeExprMethodCallContext *e) override;

    virtual void visitTypeExprMethodCallStatic(arl::dm::ITypeExprMethodCallStatic *e) override;

private:
    using ScopeInfo=std::pair<arl::dm::ITypeProcStmtScope *, int32_t>;

private:
    static dmgr::IDebug             *m_dbg;
    arl::dm::IContext               *m_ctxt;
    int32_t                         m_tmpvar_id;
    std::vector<ScopeInfo>          m_scope_s;
    vsc::dm::ITypeExpr              *m_expr;

};

}
}
}
}


