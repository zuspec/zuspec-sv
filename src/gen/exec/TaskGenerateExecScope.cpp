/*
 * TaskGenerateExecScope.cpp
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
#include "vsc/dm/impl/TaskIsDataTypeStruct.h"
#include "TaskGenerate.h"
#include "TaskGenerateDataType.h"
#include "TaskGenerateExecScope.h"
#include "TaskGenerateExpr.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateExecScope::TaskGenerateExecScope(
    TaskGenerate            *gen,
    IGenRefExpr             *genref,
    IOutput                 *out) : m_dbg(0), m_gen(gen), 
        m_genref(genref), m_out_top(out), m_exec(0) {
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateExecScope", gen->getDebugMgr());
}

TaskGenerateExecScope::~TaskGenerateExecScope() {

}

void TaskGenerateExecScope::generate(
        arl::dm::ITypeProcStmtScope *scope,
        bool                        newscope,
        bool                        istask) {
    DEBUG_ENTER("generate");
    OutputExecScope out(newscope, m_out_top);
    m_exec = &out;
    m_istask = istask;

    if (newscope) {
        m_out_top->println("begin");
        m_out_top->inc_ind();
    }

    m_genref->pushScope(scope);
    for (std::vector<arl::dm::ITypeProcStmtUP>::const_iterator
        it=scope->getStatements().begin();
        it!=scope->getStatements().end(); it++) {
        (*it)->accept(m_this);
    }
    m_genref->popScope();

    m_exec->apply(m_out_top);


    if (newscope) {
        m_out_top->dec_ind();
        m_out_top->println("end");
    }
    DEBUG_LEAVE("generate");
}

void TaskGenerateExecScope::generate(
        arl::dm::ITypeProcStmt      *stmt,
        bool                        istask) {
    DEBUG_ENTER("generate");
    OutputExecScope out(false, m_out_top);
    m_exec = &out;
    m_istask = istask;

    arl::dm::ITypeProcStmtScope *scope = dynamic_cast<arl::dm::ITypeProcStmtScope *>(stmt);

    if (scope) {
        m_genref->pushScope(scope);
        for (std::vector<arl::dm::ITypeProcStmtUP>::const_iterator
            it=scope->getStatements().begin();
            it!=scope->getStatements().end(); it++) {
            (*it)->accept(m_this);
        }
        m_genref->popScope();
    } else {
        stmt->accept(m_this);
    }

    m_exec->apply(m_out_top);


    DEBUG_LEAVE("generate");
    if (dynamic_cast<arl::dm::ITypeProcStmtScope *>(stmt)) {

    } else {

    }
}

void TaskGenerateExecScope::visitTypeProcStmtAssign(arl::dm::ITypeProcStmtAssign *s) {
    DEBUG_ENTER("visitTypeProcStmtAssign");
    m_exec->exec()->indent();
    m_exec->exec()->write("%s", m_genref->genLval(s->getLhs()).c_str());
    m_exec->exec()->write(" = ");
    TaskGenerateExpr(m_gen, m_genref, m_exec->exec()).generate(s->getRhs());
    m_exec->exec()->write(";\n");
    DEBUG_LEAVE("visitTypeProcStmtAssign");
}

void TaskGenerateExecScope::visitTypeProcStmtIfClause(arl::dm::ITypeProcStmtIfClause *s) {
    DEBUG_ENTER("visitTypeProcStmtIfClause");

    DEBUG_LEAVE("visitTypeProcStmtIfClause");
}

void TaskGenerateExecScope::visitTypeProcStmtIfElse(arl::dm::ITypeProcStmtIfElse *s) {
    DEBUG_ENTER("visitTypeProcStmtIfElse");
    for (std::vector<arl::dm::ITypeProcStmtIfClauseUP>::const_iterator
        it=s->getIfClauses().begin();
        it!=s->getIfClauses().end(); it++) {
        m_exec->exec()->indent();
        m_exec->exec()->write("%sif (", (it!=s->getIfClauses().begin())?"end else ":"");
        TaskGenerateExpr(m_gen, m_genref, m_exec->exec()).generate((*it)->getCond());
        m_exec->exec()->write(") begin\n");
        m_exec->exec()->inc_ind();
        TaskGenerateExecScope(m_gen, m_genref, m_exec->exec()).generate((*it)->getStmt());
        m_exec->exec()->dec_ind();
    }

    if (s->getElseClause()) {
        m_exec->exec()->println("end else begin");
        m_exec->exec()->inc_ind();
        TaskGenerateExecScope(m_gen, m_genref, m_exec->exec()).generate(s->getElseClause());
        m_exec->exec()->dec_ind();
    }
    m_exec->exec()->println("end");

    DEBUG_LEAVE("visitTypeProcStmtIfElse");
}

void TaskGenerateExecScope::visitTypeProcStmtExpr(arl::dm::ITypeProcStmtExpr *s) {
    DEBUG_ENTER("visitTypeProcStmtExpr");
    m_exec->exec()->indent();
    TaskGenerateExpr(m_gen, m_genref, m_exec->exec()).generate(s->getExpr());
    m_exec->exec()->write(";\n");
    DEBUG_LEAVE("visitTypeProcStmtExpr");
}

void TaskGenerateExecScope::visitTypeProcStmtReturn(arl::dm::ITypeProcStmtReturn *s) {
    DEBUG_ENTER("visitTypeProcStmtReturn %p", s);
    DEBUG("expr: %p", s->getExpr());

    if (m_istask) {
        if (s->getExpr()) {
            m_exec->exec()->indent();
            m_exec->exec()->write("__ret = ");
            TaskGenerateExpr(m_gen, m_genref, m_exec->exec()).generate(s->getExpr());
            m_exec->exec()->write(";\n");
        }
        m_exec->exec()->println("return;");
    } else {
        if (s->getExpr()) {
            m_exec->exec()->indent();
            m_exec->exec()->write("return ");
            TaskGenerateExpr(m_gen, m_genref, m_exec->exec()).generate(s->getExpr());
            m_exec->exec()->write(";\n");
        } else {
            m_exec->exec()->println("return;");
        }
    }
    DEBUG_LEAVE("visitTypeProcStmtReturn");
}

void TaskGenerateExecScope::visitTypeProcStmtVarDecl(arl::dm::ITypeProcStmtVarDecl *t) {
    DEBUG_ENTER("visitTypeProcStmtVarDecl");
    m_exec->decl()->indent();
    TaskGenerateDataType(m_gen, m_exec->decl()).generate(t->getDataType());
    // TODO: must 'new' if an aggregate type
    if (vsc::dm::TaskIsDataTypeStruct().check(t->getDataType())) {
        m_exec->decl()->write(" %s = new();\n", t->name().c_str());
    } else {
        m_exec->decl()->write(" %s;\n", t->name().c_str());
    }
    DEBUG_LEAVE("visitTypeProcStmtVarDecl");
}
 
}
}
}
}
