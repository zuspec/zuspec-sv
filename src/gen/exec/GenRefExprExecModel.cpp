/*
 * GenRefExprExecModel.cpp
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
#include "vsc/dm/impl/TaskIsTypeFieldRef.h"
#include "zsp/arl/dm/impl/TaskGetSubField.h"
#include "GenRefExprExecModel.h"
#include "TaskGenerate.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


GenRefExprExecModel::GenRefExprExecModel(
        TaskGenerate                *gen,
        vsc::dm::IDataTypeStruct    *ctxt,
        const std::string           &ctxtRef,
        bool                        ctxtPtr,
        const std::string           &bupRef,
        bool                        bupPtr) :
        m_gen(gen), m_ctxt(ctxt), m_ctxtRef(ctxtRef), m_ctxtPtr(ctxtPtr),
        m_bupRef(bupRef), m_bupPtr(bupPtr) {
    DEBUG_INIT("zsp::be::sw::GenRefExprExecModel", gen->getDebugMgr());

}

GenRefExprExecModel::~GenRefExprExecModel() {

}

std::string GenRefExprExecModel::genLval(vsc::dm::ITypeExpr *ref) {
    m_ret.clear();
    m_depth = 0;
    m_isRef = m_ctxtPtr;
    ref->accept(m_this);
    return m_ret;
}

std::string GenRefExprExecModel::genRval(vsc::dm::ITypeExpr *ref) {
    m_ret.clear();
    m_depth = 0;
    m_isRef = m_ctxtPtr;
    ref->accept(m_this);
    return m_ret;
}

bool GenRefExprExecModel::isFieldRefExpr(vsc::dm::ITypeExpr *ref) {
    DEBUG_ENTER("isFieldRefExpr");
    m_ret.clear();
    m_depth = 0;
    m_isFieldRef = false;
    m_isRefFieldRef = false;
    ref->accept(m_this);
    DEBUG_LEAVE("isFieldRefExpr %d", m_isFieldRef);
    return m_isFieldRef;
}

bool GenRefExprExecModel::isRefFieldRefExpr(vsc::dm::ITypeExpr *ref) {
    DEBUG_ENTER("isRefFieldRefExpr");
    m_ret.clear();
    m_depth = 0;
    m_isFieldRef = false;
    m_isRefFieldRef = false;
    ref->accept(m_this);
    DEBUG_LEAVE("isRefFieldRefExpr %d", m_isRefFieldRef);
    return m_isRefFieldRef;
}

IGenRefExpr::ResT GenRefExprExecModel::isRefCountedField(vsc::dm::IAccept *ref) {
    DEBUG_ENTER("isRefCountedField");
    m_ret.clear();
    m_depth = 0;
    m_isFieldRef = false;
    m_isRefFieldRef = false;
    m_isRefCountedField = false;
    ref->accept(m_this);
    DEBUG_LEAVE("isRefCountedField");
    return IGenRefExpr::ResT(m_isRefCountedField, m_type);
}

void GenRefExprExecModel::visitDataTypeAddrClaim(arl::dm::IDataTypeAddrClaim *t) {
    DEBUG_ENTER("visitDataTypeAddrClaim");
    m_isRefCountedField = false;
    DEBUG_LEAVE("visitDataTypeAddrClaim");
}

void GenRefExprExecModel::visitDataTypeAddrHandle(arl::dm::IDataTypeAddrHandle *t) {
    DEBUG_ENTER("visitDataTypeAddrHandle");
    m_isRefCountedField = true;
    DEBUG_LEAVE("visitDataTypeAddrHandle");
}

void GenRefExprExecModel::visitTypeExprRefBottomUp(vsc::dm::ITypeExprRefBottomUp *e) {
    DEBUG_ENTER("visitTypeExprRefBottomUp (%d)", m_depth);
    arl::dm::ITypeProcStmtScope *scope = m_scope_s.at(
        m_scope_s.size()-e->getScopeOffset()-1
    );
    arl::dm::ITypeProcStmtVarDecl *var = scope->getVariables().at(e->getSubFieldIndex()).get();
    DEBUG("var: %s", var->name().c_str());
    if (m_bupRef.size()) {
        m_ret.append(m_bupRef);
        m_ret.append(m_bupPtr?"->":".");
    }
    m_ret.append(var->name());
    m_isFieldRef = true;
    m_isRefFieldRef = false;
    m_type = var->getDataType();

    m_type->accept(m_this);

    // Track whether the next deref will be a pointer
    m_isRef = false;

    if (m_depth) {
        // TODO: this depends on whether the reference is scalar or
        // composite, and whether it's scope local or a parameter
        m_ret.append(".");
    }

    DEBUG_LEAVE("visitTypeExprRefBottomUp (%d)", m_depth);
}

void GenRefExprExecModel::visitTypeExprRefPath(vsc::dm::ITypeExprRefPath *e) { 
    DEBUG_ENTER("visitTypeExprRefPath");

    DEBUG_LEAVE("visitTypeExprRefPath");
}

void GenRefExprExecModel::visitTypeExprRefTopDown(vsc::dm::ITypeExprRefTopDown *e) { 
    DEBUG_ENTER("visitTypeExprRefTopDown");
    m_ret.append(m_ctxtRef);
    m_type = m_ctxt;

    m_isFieldRef = true;
    m_isRefFieldRef = false;
    // Track whether the next deref will be a pointer
    m_isRef = false;

    if (m_depth) {
        m_ret.append(m_ctxtPtr?"->":".");
    }
    DEBUG_LEAVE("visitTypeExprRefTopDown");
}

void GenRefExprExecModel::visitTypeExprSubField(vsc::dm::ITypeExprSubField *e) { 
    DEBUG_ENTER("visitTypeExprSubField (%d)", m_depth);
    m_depth++;
    e->getRootExpr()->accept(m_this);
    m_depth--;

    vsc::dm::ITypeField *field = arl::dm::TaskGetSubField().get(
        m_type, 
        e->getSubFieldIndex());
    field->accept(m_this);
    m_ret.append(field->name());
    DEBUG("field: %s", field->name().c_str());
    m_type = field->getDataType();

    if (m_depth) {
        // TODO: should determine based on field type
        m_ret.append((m_isRef)?"->":".");
    }

    // Track whether the next deref will be a pointer
    m_isRef = vsc::dm::TaskIsTypeFieldRef().eval(field);

    DEBUG_LEAVE("visitTypeExprSubField");
}

void GenRefExprExecModel::visitTypeField(vsc::dm::ITypeField *f) {
    DEBUG_ENTER("visitTypeField");
    m_isFieldRef = true;
    DEBUG_LEAVE("visitTypeField");
}

void GenRefExprExecModel::visitTypeFieldPhy(vsc::dm::ITypeFieldPhy *f) {
    DEBUG_ENTER("visitTypeFieldPhy");
    m_isFieldRef = true;
    DEBUG_LEAVE("visitTypeFieldPhy");
}

void GenRefExprExecModel::visitTypeFieldRef(vsc::dm::ITypeFieldRef *f) {
    DEBUG_ENTER("visitTypeFieldRef");
    m_isFieldRef = true;
    m_isRefFieldRef = true;
    m_isRefCountedField = true;
    DEBUG_LEAVE("visitTypeFieldRef");
}

dmgr::IDebug *GenRefExprExecModel::m_dbg = 0;

}
}
}
}
