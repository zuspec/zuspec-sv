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
#include "ICustomGen.h"
#include "TaskGenerate.h"
#include "TaskGenerateDataType.h"
#include "TaskGenerateExecScope.h"
#include "TaskGenerateExpr.h"
#include "TaskGenerateVarInit.h"
#include "TaskHasRefCountFields.h"
#include "TaskIsRefCountField.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateExecScope::TaskGenerateExecScope(
    TaskGenerate            *gen,
    IGenRefExpr             *genref,
    IOutput                 *out) : m_dbg(0), m_gen(gen), 
        m_genref(genref), m_out_top(out) {
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
    m_exec_s.push_back(&out);
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

    m_exec_s.back()->apply(m_out_top);


    if (newscope) {
        m_out_top->dec_ind();
        m_out_top->println("end");
    }
    m_exec_s.pop_back();
    DEBUG_LEAVE("generate");
}

void TaskGenerateExecScope::generate(
        arl::dm::ITypeProcStmt      *stmt,
        bool                        istask) {
    DEBUG_ENTER("generate");
    OutputExecScope out(false, m_out_top);
    m_exec_s.push_back(&out);
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

    m_exec_s.back()->apply(m_out_top);

    m_exec_s.pop_back();

    DEBUG_LEAVE("generate");
}

void TaskGenerateExecScope::visitTypeProcStmtAssign(arl::dm::ITypeProcStmtAssign *s) {
    DEBUG_ENTER("visitTypeProcStmtAssign");
    if (s->op() == arl::dm::TypeProcStmtAssignOp::Eq) {
        if (m_genref->isRefCountedField(s->getLhs()).first) {
            m_exec_s.back()->exec()->indent();
            m_exec_s.back()->exec()->write("`zsp_dec(");
            TaskGenerateExpr(m_gen, m_genref, m_exec_s.back()->exec()).generate(s->getLhs());
            m_exec_s.back()->exec()->write(");\n");
            m_exec_s.back()->exec()->indent();
            TaskGenerateExpr(m_gen, m_genref, m_exec_s.back()->exec()).generate(s->getLhs());
            m_exec_s.back()->exec()->write(" = ");
            TaskGenerateExpr(m_gen, m_genref, m_exec_s.back()->exec()).generate(s->getRhs());
            m_exec_s.back()->exec()->write(";\n");
            m_exec_s.back()->exec()->indent();
            m_exec_s.back()->exec()->write("`zsp_inc(");
            TaskGenerateExpr(m_gen, m_genref, m_exec_s.back()->exec()).generate(s->getLhs());
            m_exec_s.back()->exec()->write(");\n");
        } else if (m_genref->isAggregateFieldRefExpr(s->getLhs())) {
            m_exec_s.back()->exec()->indent();
            TaskGenerateExpr(m_gen, m_genref, m_exec_s.back()->exec()).generate(s->getLhs());
            m_exec_s.back()->exec()->write(".__assign__(");
            TaskGenerateExpr(m_gen, m_genref, m_exec_s.back()->exec()).generate(s->getRhs());
            m_exec_s.back()->exec()->write(");\n");
        } else {
            m_exec_s.back()->exec()->indent();
            TaskGenerateExpr(m_gen, m_genref, m_exec_s.back()->exec()).generate(s->getLhs());
            m_exec_s.back()->exec()->write(" = ");
            TaskGenerateExpr(m_gen, m_genref, m_exec_s.back()->exec()).generate(s->getRhs());
            m_exec_s.back()->exec()->write(";\n");
        }
    } else {
        const char *op = 0;

        // Other operators only work on numeric types
        m_exec_s.back()->exec()->indent();
        m_exec_s.back()->exec()->write("%s", m_genref->genLval(s->getLhs()).c_str());
        switch (s->op()) {
            case arl::dm::TypeProcStmtAssignOp::Eq: op = "="; break;
            case arl::dm::TypeProcStmtAssignOp::PlusEq: op = "+="; break;
            case arl::dm::TypeProcStmtAssignOp::MinusEq: op = "-="; break;
            case arl::dm::TypeProcStmtAssignOp::ShlEq: op = "<<="; break;
            case arl::dm::TypeProcStmtAssignOp::ShrEq: op = ">>="; break;
            case arl::dm::TypeProcStmtAssignOp::OrEq: op = "|="; break;
            case arl::dm::TypeProcStmtAssignOp::AndEq: op = "&="; break;
        }
        DEBUG("op: %s (%d)", op, s->op());
        m_exec_s.back()->exec()->write(" ");
        m_exec_s.back()->exec()->write(op);
        m_exec_s.back()->exec()->write(" ");

        TaskGenerateExpr(m_gen, m_genref, m_exec_s.back()->exec()).generate(s->getRhs());
        m_exec_s.back()->exec()->write(";\n");
    }

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
        m_exec_s.back()->exec()->indent();
        m_exec_s.back()->exec()->write("%sif (", (it!=s->getIfClauses().begin())?"end else ":"");
        TaskGenerateExpr(m_gen, m_genref, m_exec_s.back()->exec()).generate((*it)->getCond());
        m_exec_s.back()->exec()->write(") begin\n");
        m_exec_s.back()->exec()->inc_ind();
        TaskGenerateExecScope(m_gen, m_genref, m_exec_s.back()->exec()).generate((*it)->getStmt());
        m_exec_s.back()->exec()->dec_ind();
    }

    if (s->getElseClause()) {
        m_exec_s.back()->exec()->println("end else begin");
        m_exec_s.back()->exec()->inc_ind();
        TaskGenerateExecScope(m_gen, m_genref, m_exec_s.back()->exec()).generate(s->getElseClause());
        m_exec_s.back()->exec()->dec_ind();
    }
    m_exec_s.back()->exec()->println("end");

    DEBUG_LEAVE("visitTypeProcStmtIfElse");
}

void TaskGenerateExecScope::visitTypeProcStmtExpr(arl::dm::ITypeProcStmtExpr *s) {
    DEBUG_ENTER("visitTypeProcStmtExpr");
    m_exec_s.back()->exec()->indent();
    TaskGenerateExpr(m_gen, m_genref, m_exec_s.back()->exec()).generate(s->getExpr());
    m_exec_s.back()->exec()->write(";\n");
    DEBUG_LEAVE("visitTypeProcStmtExpr");
}

void TaskGenerateExecScope::visitTypeProcStmtForeach(arl::dm::ITypeProcStmtForeach *s) {
    DEBUG_ENTER("visitTypeProcStmtForeach");
//    m_genref->pushScope(s);

//    m_genref->popScope();
    DEBUG_LEAVE("visitTypeProcStmtForeach");
}

void TaskGenerateExecScope::visitTypeProcStmtRepeat(arl::dm::ITypeProcStmtRepeat *s) {
    DEBUG_ENTER("visitTypeProcStmtRepeat");

    m_genref->pushScope(s);

    // TODO: handle index variable

    vsc::dm::ITypeVar *idx_v = 0;

    // Null field indicates that the index isn't used
    if (s->getNumVariables() && s->getVariables().at(0)) {
        idx_v = s->getVariables().at(0).get();
    }

    if (idx_v) {
        std::string ivname = idx_v->name();

        if (ivname == "_") {
            ivname = "__xyz";
        }
        m_exec_s.back()->exec()->indent();
        m_exec_s.back()->exec()->write("for (");
        TaskGenerateDataType(m_gen, m_exec_s.back()->exec()).generate(idx_v->getDataType());
        m_exec_s.back()->exec()->write(" %s=0; %s<", ivname.c_str(), ivname.c_str());
        TaskGenerateExpr(m_gen, m_genref, m_exec_s.back()->exec()).generate(s->getExpr());
        m_exec_s.back()->exec()->write("; %s+=1) begin\n", ivname.c_str());
        m_exec_s.back()->exec()->inc_ind();
        /*
        for (std::vector<arl::dm::ITypeProcStmtVarDeclUP>::const_iterator
            it=s->getVariables().begin()+1;
            it!=s->getVariables().end(); it++) {
            (*it)->accept(m_this);
        }
         */
    } else {
        m_exec_s.back()->exec()->indent();
        m_exec_s.back()->exec()->write("repeat (");
        TaskGenerateExpr(m_gen, m_genref, m_exec_s.back()->exec()).generate(s->getExpr());
        m_exec_s.back()->exec()->write(") begin\n");
        m_exec_s.back()->exec()->inc_ind();
    }

    s->getBody()->accept(m_this);

    m_exec_s.back()->exec()->dec_ind();
    m_exec_s.back()->exec()->println("end");

    m_genref->popScope();

    DEBUG_LEAVE("visitTypeProcStmtRepeat");
}

void TaskGenerateExecScope::visitTypeProcStmtRepeatWhile(arl::dm::ITypeProcStmtRepeatWhile *s) {
    DEBUG_ENTER("visitTypeProcStmtRepeatWhile");
    m_exec_s.back()->exec()->println("do begin");
    m_exec_s.back()->exec()->inc_ind();
    s->getBody()->accept(m_this);
    m_exec_s.back()->exec()->dec_ind();
    m_exec_s.back()->exec()->indent();
    m_exec_s.back()->exec()->write("end while(");
    TaskGenerateExpr(m_gen, m_genref, m_exec_s.back()->exec()).generate(s->getExpr());
    m_exec_s.back()->exec()->write(");\n");
    DEBUG_LEAVE("visitTypeProcStmtRepeatWhile");
}

void TaskGenerateExecScope::visitTypeProcStmtReturn(arl::dm::ITypeProcStmtReturn *s) {
    DEBUG_ENTER("visitTypeProcStmtReturn %p", s);
    DEBUG("expr: %p", s->getExpr());

    if (m_istask) {
        if (s->getExpr()) {
            m_exec_s.back()->exec()->indent();
            m_exec_s.back()->exec()->write("__ret = ");
            TaskGenerateExpr(m_gen, m_genref, m_exec_s.back()->exec()).generate(s->getExpr());
            m_exec_s.back()->exec()->write(";\n");
        }
        m_exec_s.back()->exec()->println("return;");
    } else {
        if (s->getExpr()) {
            m_exec_s.back()->exec()->indent();
            m_exec_s.back()->exec()->write("return ");
            TaskGenerateExpr(m_gen, m_genref, m_exec_s.back()->exec()).generate(s->getExpr());
            m_exec_s.back()->exec()->write(";\n");
        } else {
            m_exec_s.back()->exec()->println("return;");
        }
    }
    DEBUG_LEAVE("visitTypeProcStmtReturn");
}

void TaskGenerateExecScope::visitTypeProcStmtScope(arl::dm::ITypeProcStmtScope *s) {
    DEBUG_ENTER("visitTypeProcStmtScope");
    m_genref->pushScope(s);
    OutputExecScope out(false, m_exec_s.back()->exec());
    m_exec_s.push_back(&out);
    VisitorBase::visitTypeProcStmtScope(s);
    m_exec_s.pop_back();
    out.apply(m_exec_s.back()->exec());
    m_genref->popScope();
    DEBUG_LEAVE("visitTypeProcStmtScope");
}

void TaskGenerateExecScope::visitTypeProcStmtVarDecl(arl::dm::ITypeProcStmtVarDecl *t) {
    DEBUG_ENTER("visitTypeProcStmtVarDecl");
    m_exec_s.back()->decl()->indent();
    TaskGenerateDataType(m_gen, m_exec_s.back()->decl()).generate(t->getDataType());
    m_exec_s.back()->decl()->write(" %s;\n", t->name().c_str());

    ICustomGen *cgen = dynamic_cast<ICustomGen *>(t->getDataType()->getAssociatedData());

    if (cgen) {
        cgen->genVarDeclInit(m_gen, m_exec_s.back()->init(), m_genref, t);
    } else {
        TaskGenerateVarInit(m_gen, m_genref, m_exec_s.back()->init()).generate(t);
    }

    if (TaskHasRefCountFields().check(t)) {
        m_exec_s.back()->dtor()->println("`zsp_dec(%s);", t->name().c_str());
    }

    DEBUG_LEAVE("visitTypeProcStmtVarDecl");
}

void TaskGenerateExecScope::visitTypeProcStmtWhile(arl::dm::ITypeProcStmtWhile *s) {
    DEBUG_ENTER("visitTypeProcStmtWhile");
    m_exec_s.back()->exec()->indent();
    m_exec_s.back()->exec()->write("while (");
    TaskGenerateExpr(m_gen, m_genref, m_exec_s.back()->exec()).generate(s->getCond());
    m_exec_s.back()->exec()->write(") begin\n");
    m_exec_s.back()->exec()->inc_ind();
    s->getBody()->accept(m_this);
    m_exec_s.back()->exec()->dec_ind();
    m_exec_s.back()->exec()->println("end");
    DEBUG_LEAVE("visitTypeProcStmtWhile");
}

void TaskGenerateExecScope::visitTypeProcStmtYield(arl::dm::ITypeProcStmtYield *s) {
    m_exec_s.back()->exec()->println("#0;");
}
 
}
}
}
}
