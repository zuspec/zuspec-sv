/*
 * TaskGenerateExpr.cpp
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
#include <map>
#include "dmgr/impl/DebugMacros.h"
#include "ICustomGen.h"
#include "TaskGenerate.h"
#include "TaskGenerateExpr.h"
#include "TaskGenerateExprVal.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateExpr::TaskGenerateExpr(
    TaskGenerate        *gen,
    IGenRefExpr         *genref,
    IOutput             *out) : m_dbg(0), m_gen(gen), m_genref(genref), m_out(out) {
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateExpr", gen->getDebugMgr());
}

TaskGenerateExpr::~TaskGenerateExpr() {

}

void TaskGenerateExpr::generate(vsc::dm::ITypeExpr *e) {
    DEBUG_ENTER("generate");
    e->accept(m_this);
    DEBUG_LEAVE("generate");
}

static std::map<vsc::dm::BinOp, std::string> binOpMap = {
	{ vsc::dm::BinOp::Eq, "==" },
	{ vsc::dm::BinOp::Ne, "!=" },
	{ vsc::dm::BinOp::Gt, ">" },
	{ vsc::dm::BinOp::Ge, ">=" },
	{ vsc::dm::BinOp::Lt, "<" },
	{ vsc::dm::BinOp::Le, "<=" },
	{ vsc::dm::BinOp::Add, "+" },
	{ vsc::dm::BinOp::Sub, "-" },
	{ vsc::dm::BinOp::Div, "/" },
	{ vsc::dm::BinOp::Mul, "*" },
	{ vsc::dm::BinOp::Mod, "%" },
	{ vsc::dm::BinOp::BinAnd, "&" },
	{ vsc::dm::BinOp::BinOr, "|"  },
	{ vsc::dm::BinOp::BinXor, "^" },
	{ vsc::dm::BinOp::LogAnd, "&&" },
	{ vsc::dm::BinOp::LogOr, "||" },
	{ vsc::dm::BinOp::LogXor, "^^" },
	{ vsc::dm::BinOp::Sll, "<<" },
	{ vsc::dm::BinOp::Srl, ">>" },
	{ vsc::dm::BinOp::Not, "~" }
};

void TaskGenerateExpr::visitTypeExprBin(vsc::dm::ITypeExprBin *e) {
    DEBUG_ENTER("visitTypeExprBin");
    e->lhs()->accept(m_this);
    m_out->write(" %s ", 
        binOpMap.find(e->op())->second.c_str());
    e->rhs()->accept(m_this);
    DEBUG_LEAVE("visitTypeExprBin");
}

void TaskGenerateExpr::visitTypeExprMethodCallContext(
    arl::dm::ITypeExprMethodCallContext *e) {
    DEBUG_ENTER("visitTypeExprMethodCallContext");
    arl::dm::IDataTypeFunction *f = e->getTarget();
    ICustomGen *custom_g = dynamic_cast<ICustomGen *>(f->getAssociatedData());

    if (custom_g) {
        custom_g->genExprMethodCallContext(
            m_gen,
            m_out,
            m_genref,
            e);
    } else {
        std::string name = e->getTarget()->name();
        int idx = name.rfind("::");
        if (idx != -1) {
            name = name.substr(idx+2);
        }
        m_out->write("%s.%s(",
            m_genref->genRval(e->getContext()).c_str(),
            name.c_str());
        for (std::vector<vsc::dm::ITypeExprUP>::const_iterator
            it=e->getParameters().begin();
            it!=e->getParameters().end(); it++) {
            if (it != e->getParameters().begin()) {
                m_out->write(", ");
            }
            TaskGenerateExpr(m_gen, m_genref, m_out).generate(it->get());
        }
        m_out->write(")");
    }
    DEBUG_LEAVE("visitTypeExprMethodCallContext");
}

void TaskGenerateExpr::visitTypeExprMethodCallStatic(
    arl::dm::ITypeExprMethodCallStatic *e) {
    arl::dm::IDataTypeFunction *f = e->getTarget();
    ICustomGen *custom_g = dynamic_cast<ICustomGen *>(f->getAssociatedData());

    if (custom_g) {
        custom_g->genExprMethodCallStatic(
            m_gen,
            m_out,
            m_genref,
            e);
    } else {
        std::string name = e->getTarget()->name();
        int idx = name.rfind("::");
        if (idx != -1) {
            name = name.substr(idx+2);
        }
        m_out->write("%s(", name.c_str());
        for (std::vector<vsc::dm::ITypeExprUP>::const_iterator
            it=e->getParameters().begin();
            it!=e->getParameters().end(); it++) {
            if (it != e->getParameters().begin()) {
                m_out->write(", ");
            }
            TaskGenerateExpr(m_gen, m_genref, m_out).generate(it->get());
        }
        m_out->write(")");
    }
}

void TaskGenerateExpr::visitTypeExprRange(vsc::dm::ITypeExprRange *e) { 

}

void TaskGenerateExpr::visitTypeExprRangelist(vsc::dm::ITypeExprRangelist *e) { 

}

void TaskGenerateExpr::visitTypeExprRefBottomUp(vsc::dm::ITypeExprRefBottomUp *e) { 
    DEBUG_ENTER("visitTypeExprRefBottomUp");
    m_out->write("%s", m_genref->genRval(e).c_str());
    DEBUG_LEAVE("visitTypeExprRefBottomUp");
}

void TaskGenerateExpr::visitTypeExprRefTopDown(vsc::dm::ITypeExprRefTopDown *e) { 
    DEBUG_ENTER("visitTypeExprRefTopDown");
    m_out->write("%s", m_genref->genRval(e).c_str());
    DEBUG_LEAVE("visitTypeExprRefTopDown");
}

void TaskGenerateExpr::visitTypeExprSubField(vsc::dm::ITypeExprSubField *e) { 
    DEBUG_ENTER("visitTypeExprSubField");
    m_out->write("%s", m_genref->genRval(e).c_str());
    DEBUG_LEAVE("visitTypeExprSubField");
}

void TaskGenerateExpr::visitTypeExprUnary(vsc::dm::ITypeExprUnary *e) { 

}

void TaskGenerateExpr::visitTypeExprVal(vsc::dm::ITypeExprVal *e) { 
    TaskGenerateExprVal(m_gen, m_out).generate(e);
}

}
}
}
}
