/*
 * TaskGenerateStructConstraints.cpp
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
#include "TaskGenerate.h"
#include "TaskGenerateExpr.h"
#include "TaskGenerateStructConstraints.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateStructConstraints::TaskGenerateStructConstraints(
    TaskGenerate        *gen,
    IGenRefExpr         *genref,
    IOutput             *out) : m_dbg(0), m_gen(gen), 
        m_genref(genref), m_out(out) {
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateStructConstraints", gen->getDebugMgr());
}

TaskGenerateStructConstraints::~TaskGenerateStructConstraints() {

}

void TaskGenerateStructConstraints::generate(vsc::dm::IDataTypeStruct *t) {
    for (std::vector<vsc::dm::ITypeConstraintUP>::const_iterator
        it=t->getConstraints().begin();
        it!=t->getConstraints().end(); it++) {
        (*it)->accept(m_this);
    }
}

void TaskGenerateStructConstraints::visitTypeConstraintBlock(vsc::dm::ITypeConstraintBlock *c) { 
    DEBUG_ENTER("visitTypeConstraintBlock (%d)", c->getConstraints().size());
    std::string name = c->name();

    if (name == "") {
        char tmp[128];
        sprintf(tmp, "c_%p", c);
        name = tmp;
    }

    m_out->println("constraint %s {", name.c_str());
    m_out->inc_ind();
    for (std::vector<vsc::dm::ITypeConstraintUP>::const_iterator
        it=c->getConstraints().begin();
        it!=c->getConstraints().end(); it++) {
        (*it)->accept(m_this);
    }
    m_out->dec_ind();
    m_out->println("}");

    DEBUG_LEAVE("visitTypeConstraintBlock");
}

void TaskGenerateStructConstraints::visitTypeConstraintExpr(vsc::dm::ITypeConstraintExpr *c) { 
    m_out->indent();
    TaskGenerateExpr(m_gen, m_genref, m_out).generate(c->expr());
    m_out->write(";\n");
}

void TaskGenerateStructConstraints::visitTypeConstraintForeach(vsc::dm::ITypeConstraintForeach *c) { }

void TaskGenerateStructConstraints::visitTypeConstraintIfElse(vsc::dm::ITypeConstraintIfElse *c) { 
    m_out->indent();
    m_out->write("if (");
    TaskGenerateExpr(m_gen, m_genref, m_out).generate(c->getCond());
    m_out->write(") {\n");
    m_out->inc_ind();
    c->getTrue()->accept(m_this);
    m_out->dec_ind();
    if (c->getFalse()) {
        m_out->println("} else {");
        m_out->inc_ind();
        c->getFalse()->accept(m_this);
        m_out->dec_ind();
    }
    m_out->println("}\n");
}

void TaskGenerateStructConstraints::visitTypeConstraintImplies(vsc::dm::ITypeConstraintImplies *c) { 
    m_out->indent();
    m_out->write("(");
    TaskGenerateExpr(m_gen, m_genref, m_out).generate(c->getCond());
    m_out->write(") -> {\n");
    m_out->inc_ind();
    c->getBody()->accept(m_this);
    m_out->dec_ind();
    m_out->println("}");
}

void TaskGenerateStructConstraints::visitTypeConstraintSoft(vsc::dm::ITypeConstraintSoft *c) { }

void TaskGenerateStructConstraints::visitTypeConstraintUnique(vsc::dm::ITypeConstraintUnique *c) { }

}
}
}
}
