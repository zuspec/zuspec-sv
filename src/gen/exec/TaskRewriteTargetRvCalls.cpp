/*
 * TaskRewriteTargetRvCalls.cpp
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
#include "TaskHasTargetRvCalls.h"
#include "TaskRewriteTargetRvCalls.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskRewriteTargetRvCalls::TaskRewriteTargetRvCalls(
    dmgr::IDebugMgr         *dmgr,
    arl::dm::IContext       *ctxt) : m_ctxt(ctxt), m_tmpvar_id(0) {
    DEBUG_INIT("zsp::sv::gen::exec::TaskRewriteTargetRvCalls", dmgr);
}

TaskRewriteTargetRvCalls::~TaskRewriteTargetRvCalls() {

}

void TaskRewriteTargetRvCalls::rewrite(arl::dm::ITypeProcStmtScope *scope) {
    DEBUG_ENTER("rewrite");
    m_scope_s.push_back({scope, 0});
    for (; m_scope_s.back().second < scope->getStatements().size(); 
            m_scope_s.back().second++) {
        scope->getStatements().at(m_scope_s.back().second)->accept(m_this);
    }
    m_scope_s.pop_back();
    DEBUG_LEAVE("rewrite");
}

void TaskRewriteTargetRvCalls::visitTypeExprArrIndex(vsc::dm::ITypeExprArrIndex *e) { }

void TaskRewriteTargetRvCalls::visitTypeExprBin(vsc::dm::ITypeExprBin *e) {
    DEBUG_ENTER("visitTypeExprBin");

    DEBUG_LEAVE("visitTypeExprBin");
}

void TaskRewriteTargetRvCalls::visitTypeExprFieldRef(vsc::dm::ITypeExprFieldRef *e) { 
    DEBUG_ENTER("visitTypeExprFieldRef");

    DEBUG_LEAVE("visitTypeExprFieldRef");
}

void TaskRewriteTargetRvCalls::visitTypeExprRange(vsc::dm::ITypeExprRange *e) { 
    DEBUG_ENTER("visitTypeExprRange");

    DEBUG_LEAVE("visitTypeExprRange");
}

void TaskRewriteTargetRvCalls::visitTypeExprRangelist(vsc::dm::ITypeExprRangelist *e) { 
    DEBUG_ENTER("visitTypeExprRangelist");

    DEBUG_LEAVE("visitTypeExprRangelist");
}

void TaskRewriteTargetRvCalls::visitTypeExprRef(vsc::dm::ITypeExprRef *e) { 
    DEBUG_ENTER("visitTypeExprRef");

    DEBUG_LEAVE("visitTypeExprRef");
}

void TaskRewriteTargetRvCalls::visitTypeExprUnary(vsc::dm::ITypeExprUnary *e) { 
    DEBUG_ENTER("visitTypeExprUnary");

    m_expr = 0;
    e->accept(m_this);

    if (m_expr) {
        DEBUG("TODO: replace unary");
    }
    DEBUG_LEAVE("visitTypeExprUnary");
}

void TaskRewriteTargetRvCalls::visitTypeExprMethodCallContext(arl::dm::ITypeExprMethodCallContext *e) { 
    DEBUG_ENTER("visitTypeExprMethodCallContext");
    if (TaskHasTargetRvCalls().check(e)) {
        // Insert two statements
        // - Var declaration for the tempvar
        // - Statement to call the target function
        char name_t[256];

        snprintf(name_t, sizeof(name_t), "targ_t%d", m_tmpvar_id);
        m_tmpvar_id++;

        arl::dm::ITypeProcStmtVarDecl *var = m_ctxt->mkTypeProcStmtVarDecl(
            name_t,
            e->getTarget()->getReturnType(),
            false,
            0);

        int32_t var_id = m_scope_s.back().first->insertVariable( // Var decl
            m_scope_s.back().second,
            var);
        m_scope_s.back().second += 1;

        std::vector<vsc::dm::ITypeExpr *> params;

        // First parameter is the return-value variable
        params.push_back(m_ctxt->mkTypeExprRefBottomUp(0, var_id));

        for (std::vector<vsc::dm::ITypeExprUP>::iterator
            it=e->getParameters().begin();
            it!=e->getParameters().end(); it++) {
            m_expr = 0;
            (*it)->accept(m_this);

            if (m_expr) {
                params.push_back(m_expr);
            } else {
                params.push_back(it->release());
            }
        }

        arl::dm::ITypeExprMethodCallContext *call = m_ctxt->mkTypeExprMethodCallContext(
            e->getTarget(),
            e->rlsContext(),
            params);

        m_scope_s.back().first->insertStatement( // Call statement
            m_scope_s.back().second,
            m_ctxt->mkTypeProcStmtExpr(call)
        );

        m_scope_s.back().second += 1;

        // Change assign to reference tempvar instead
        m_expr = m_ctxt->mkTypeExprSubField(
            m_ctxt->mkTypeExprRefBottomUp(0, 0),
            true,
            var_id);
    }
    DEBUG_LEAVE("visitTypeExprMethodCallContext");
}

void TaskRewriteTargetRvCalls::visitTypeExprMethodCallStatic(arl::dm::ITypeExprMethodCallStatic *e) { 
    DEBUG_ENTER("visitTypeExprMethodCallStatic 0x%08x %p", 
        e->getTarget()->getFlags(),
        e->getTarget()->getReturnType());
    if (TaskHasTargetRvCalls().check(e)) {
        DEBUG("-- rewrite");
        // Insert two statements
        // - Var declaration for the tempvar
        // - Statement to call the target function
        char name_t[256];

        snprintf(name_t, sizeof(name_t), "targ_t%d", m_tmpvar_id);
        m_tmpvar_id++;

        arl::dm::ITypeProcStmtVarDecl *var = m_ctxt->mkTypeProcStmtVarDecl(
            name_t,
            e->getTarget()->getReturnType(),
            false,
            0);

        int32_t var_id = m_scope_s.back().first->insertVariable( // Var decl
            m_scope_s.back().second,
            var);
        m_scope_s.back().second += 1;

        std::vector<vsc::dm::ITypeExpr *> params;

        // First parameter is the return-value variable
        params.push_back(m_ctxt->mkTypeExprRefBottomUp(0, var_id));

        for (std::vector<vsc::dm::ITypeExprUP>::iterator
            it=e->getParameters().begin();
            it!=e->getParameters().end(); it++) {
            m_expr = 0;
            (*it)->accept(m_this);

            if (m_expr) {
                params.push_back(m_expr);
            } else {
                params.push_back(it->release());
            }
        }

        arl::dm::ITypeExprMethodCallStatic *call = m_ctxt->mkTypeExprMethodCallStatic(
            e->getTarget(),
            params);

        m_scope_s.back().first->insertStatement( // Call statement
            m_scope_s.back().second,
            m_ctxt->mkTypeProcStmtExpr(call)
        );

        m_scope_s.back().second += 1;

        // Change assign to reference tempvar instead
        m_expr = m_ctxt->mkTypeExprRefBottomUp(0, var_id);
    }
    DEBUG_LEAVE("visitTypeExprMethodCallStatic");
}

void TaskRewriteTargetRvCalls::visitTypeProcStmtAssign(arl::dm::ITypeProcStmtAssign *s) {

    m_expr = 0;
    s->getRhs()->accept(m_this);

    if (m_expr) {
        s->setRhs(m_expr);
    }
}

dmgr::IDebug *TaskRewriteTargetRvCalls::m_dbg;

}
}
}
}
