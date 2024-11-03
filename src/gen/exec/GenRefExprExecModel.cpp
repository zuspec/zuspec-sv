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
#include "zsp/arl/dm/impl/TaskIsTypeFieldRegGroup.h"
#include "GenRefExprExecModel.h"
#include "gen/OutputStr.h"
#include "TaskGenerate.h"
#include "TaskGenerateExpr.h"
#include "TaskGenerateExprVal.h"


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
    m_depth = 0;
    m_field = 0;
}

GenRefExprExecModel::~GenRefExprExecModel() {

}

std::string GenRefExprExecModel::genLval(vsc::dm::ITypeExpr *ref) {
    DEBUG_ENTER("genLval");
    init(KindE::Lval);
    ref->accept(m_this);
    std::string ret = strval();
    DEBUG_LEAVE("genLval (%s)", ret.c_str());
    return ret;
}

std::string GenRefExprExecModel::genRval(vsc::dm::ITypeExpr *ref) {
    DEBUG_ENTER("genRval");
    init(KindE::Rval);
    ref->accept(m_this);
    std::string ret = strval();
    DEBUG_LEAVE("genRval (%s)", ret.c_str());
    return ret;
}

std::string GenRefExprExecModel::genRegAddr(vsc::dm::ITypeExpr *ref) {
    DEBUG_ENTER("genRegAddr");
    init(KindE::RegAddr);
    ref->accept(m_this);
    std::string ret = strval();
    DEBUG_LEAVE("genRegAddr (%s)", ret.c_str());
    return ret;
}

bool GenRefExprExecModel::isFieldRefExpr(vsc::dm::ITypeExpr *ref) {
    DEBUG_ENTER("isFieldRefExpr");
    init(KindE::Check);
    ref->accept(m_this);
    DEBUG_LEAVE("isFieldRefExpr %d", m_isFieldRef);
    return m_isFieldRef;
}

bool GenRefExprExecModel::isRefFieldRefExpr(vsc::dm::ITypeExpr *ref) {
    DEBUG_ENTER("isRefFieldRefExpr");
    init(KindE::Check);
    ref->accept(m_this);
    DEBUG_LEAVE("isRefFieldRefExpr %d", m_isRefFieldRef);
    return m_isRefFieldRef;
}

bool GenRefExprExecModel::isAggregateFieldRefExpr(vsc::dm::ITypeExpr *ref) {
    DEBUG_ENTER("isAggregateFieldRefExpr");
    init(KindE::Check);
    ref->accept(m_this);
    DEBUG_LEAVE("isAggregateFieldRefExpr");
    return m_isAggregateFieldRef;
}

IGenRefExpr::ResT GenRefExprExecModel::isRefCountedField(vsc::dm::IAccept *ref) {
    DEBUG_ENTER("isRefCountedField");
    init(KindE::Check);
    ref->accept(m_this);
    DEBUG_LEAVE("isRefCountedField");
    return IGenRefExpr::ResT(m_isRefCountedField, m_type_l.back());
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

void GenRefExprExecModel::visitDataTypeArray(vsc::dm::IDataTypeArray *t) {
    DEBUG_ENTER("visitDataTypeArray");
    m_isAggregateFieldRef = true;
    DEBUG_LEAVE("visitDataTypeArray");
}

void GenRefExprExecModel::visitDataTypeFlowObj(arl::dm::IDataTypeFlowObj *t) {
    DEBUG_ENTER("visitDataTYpeFlowObj");
    m_isRefCountedField = true;
    DEBUG_LEAVE("visitDataTYpeFlowObj");
}

void GenRefExprExecModel::visitDataTypeStruct(vsc::dm::IDataTypeStruct *t) {
    DEBUG_ENTER("visitDataTypeStruct");
    m_isAggregateFieldRef = true;
    DEBUG_LEAVE("visitDataTypeStruct");
}

void GenRefExprExecModel::visitTypeExprArrIndex(vsc::dm::ITypeExprArrIndex *e) {
    DEBUG_ENTER("visitTypeExprArrIndex kind=%d", (int)m_kind);
    std::string ret;
    vsc::dm::IDataTypeArray *arr_t = 0;
    m_depth_ss++;
    switch (m_kind) {
        case KindE::Lval:
        case KindE::Rval:
            m_depth++;
            e->getRootExpr()->accept(m_this);
            m_depth--;

            arr_t = dynamic_cast<vsc::dm::IDataTypeArray *>(m_type_l.back());

            ret.append("[");
            e->getIndexExpr()->accept(m_this);
            ret.append("]");
            m_out_l.push_back(ret);
            break;
        case KindE::RegAddr:
            // Capture subscripts
            DEBUG("pre-root: %s", strval().c_str());
            m_depth++;
            e->getRootExpr()->accept(m_this);
            m_depth--;
            DEBUG("post-root: %s", strval().c_str());

            // The result of traversing the array reference is the
            // array type. Save that for later
            arr_t = dynamic_cast<vsc::dm::IDataTypeArray *>(m_type_l.back());

            if (m_regRef) {
                vsc::dm::IDataType *base_t = m_type_l.at(m_type_l.size()-2);
                m_out_l.pop_back();
                ret.append("`zsp_reg_type_arr_offset(");
                ret.append(m_gen->getNameMap()->getName(base_t));
                ret.append(", ");
                ret.append(m_field->name());
                ret.append(", ");
                OutputStr out;
                GenExprStandalone(&out, e->getIndexExpr());
                ret.append(out.getValue());
                ret.append(")");
                if (m_depth) {
                    ret.append(" + ");
                }
            } else {
                ret.append("[");
                e->getIndexExpr()->accept(m_this);
                ret.append("]");
            }
            DEBUG("post-index(1): %s", strval().c_str());
            m_out_l.push_back(ret);
            DEBUG("post-index(2): %s", strval().c_str());
            break;
        default:
            e->getRootExpr()->accept(m_this);
            arr_t = dynamic_cast<vsc::dm::IDataTypeArray *>(m_type_l.back());
            break;
    }

    if (!arr_t) {
        DEBUG_ERROR("Failed to identify array type");
    }

    m_type_l.pop_back();
    m_type_l.push_back(arr_t->getElemType());
    m_depth_ss--;

    DEBUG_LEAVE("visitTypeExprArrIndex");
}

void GenRefExprExecModel::visitTypeExprMethodCallContext(arl::dm:: ITypeExprMethodCallContext *e) {
    DEBUG_ENTER("visitTypeExprMethodCallContext");
    OutputStr out;
    GenExpr(&out, e);
    m_out_l.push_back(out.getValue());
    DEBUG_LEAVE("visitTypeExprMethodCallContext");
}

void GenRefExprExecModel::visitTypeExprMethodCallStatic(arl::dm::ITypeExprMethodCallStatic *e) {
    DEBUG_ENTER("visitTypeExprMethodCallStatic");
    OutputStr out;
    GenExpr(&out, e);
    m_out_l.push_back(out.getValue());
    DEBUG_LEAVE("visitTypeExprMethodCallStatic");
}

void GenRefExprExecModel::visitTypeExprRefBottomUp(vsc::dm::ITypeExprRefBottomUp *e) {
    std::string ret;
    DEBUG_ENTER("visitTypeExprRefBottomUp (%d) scopeOff=%d varOffset=%d", 
        m_depth,
        e->getScopeOffset(),
        e->getSubFieldIndex());
    vsc::dm::ITypeVarScope *scope = m_scope_s.at(
        m_scope_s.size()-e->getScopeOffset()-1
    );
    vsc::dm::ITypeVar *var = scope->getVariables().at(e->getSubFieldIndex()).get();
    DEBUG("var: %s", var->name().c_str());

    switch (m_kind) {
        case KindE::Lval:
        case KindE::Rval:
            if (m_bupRef.size()) {
                ret.append(m_bupRef);
                ret.append(".");
            }
            ret.append(var->name());

            if (m_depth) {
                // TODO: this depends on whether the reference is scalar or
                // composite, and whether it's scope local or a parameter
                ret.append(".");
            }
            m_out_l.push_back(ret);
            break;

        case KindE::RegAddr:
            if (m_regRef) {

            } else {
                if (m_bupRef.size()) {
                    ret.append(m_bupRef);
                    ret.append(".");
                }
                ret.append(var->name());

                if (m_depth) {
                    // TODO: this depends on whether the reference is scalar or
                    // composite, and whether it's scope local or a parameter
                    ret.append(".");
                }
                m_out_l.push_back(ret);
            }
            break;
    }
    m_isFieldRef = true;
    m_isRefFieldRef = false;
    m_type_l.push_back(var->getDataType());

    m_type_l.back()->accept(m_this);

    // Track whether the next deref will be a pointer
    m_isRef = false;


    DEBUG_LEAVE("visitTypeExprRefBottomUp (%d)", m_depth);
}

void GenRefExprExecModel::visitTypeExprRefInline(vsc::dm::ITypeExprRefInline *e) {
    DEBUG_ENTER("visitTypeExprRefInline");
    m_type_l.push_back((m_inline_s.size())?m_inline_s.back():0);
    DEBUG_LEAVE("visitTypeExprRefInline %p", m_type_l.back());
}

void GenRefExprExecModel::visitTypeExprRefPath(vsc::dm::ITypeExprRefPath *e) { 
    DEBUG_ENTER("visitTypeExprRefPath");

    DEBUG_LEAVE("visitTypeExprRefPath");
}

void GenRefExprExecModel::visitTypeExprRefTopDown(vsc::dm::ITypeExprRefTopDown *e) { 
    std::string ret;
    DEBUG_ENTER("visitTypeExprRefTopDown");
    switch (m_kind) {
        case KindE::Lval:
        case KindE::Rval:
            ret.append(m_ctxtRef);
            if (m_depth) {
                ret.append(".");
            }
            m_out_l.push_back(ret);
            break;
        case KindE::RegAddr:
            ret.append(m_ctxtRef);
            if (m_depth) {
                ret.append(".");
            }
            m_out_l.push_back(ret);
            break;
    }

    m_type_l.push_back(m_ctxt);

    m_isFieldRef = true;
    m_isRefFieldRef = false;
    // Track whether the next deref will be a pointer
    m_isRef = false;

    DEBUG_LEAVE("visitTypeExprRefTopDown");
}

void GenRefExprExecModel::visitTypeExprSubField(vsc::dm::ITypeExprSubField *e) { 
    std::string ret;
    DEBUG_ENTER("visitTypeExprSubField (%d) kind=%d field=%p", m_depth, (int)m_kind, m_field);

    // Note that this method may be called when `m_field` is not set
    m_depth++;
    e->getRootExpr()->accept(m_this);
    m_depth--;

    m_field = arl::dm::TaskGetSubField().get(
        m_type_l.back(),
        e->getSubFieldIndex());

    if (!m_field) {
        DEBUG("m_field is null");
    }

    DEBUG("field: %s", (m_field)?m_field->name().c_str():"<unknown>");
    bool currRegRef = m_regRef;
    m_isAggregateFieldRef = false;
    if (m_field) {
        m_field->accept(m_this);
    }

    switch (m_kind) {
        case KindE::Lval:
        case KindE::Rval:
            if (m_field) {
                ret.append(m_field->name());
            }
            if (m_depth) {
                // TODO: should determine based on field type
                ret.append(".");
            }
            m_out_l.push_back(ret);
            break;
        case KindE::RegAddr:
            if (m_regRef) {
                if (currRegRef) {
                    ret.append("`zsp_reg_type_offset(");
                    ret.append(m_gen->getNameMap()->getName(m_type_l.back()));
                    ret.append(", ");
                    ret.append(m_field->name());
                    ret.append(")");
                } else {
                    ret.append(m_field->name());
                    ret.append(".offset");
                }

                if (m_depth) {
                    ret.append(" + ");
                }
            } else {
                ret.append(m_field->name());
                if (m_depth) {
                    // TODO: should determine based on field type
                    ret.append(".");
                }
            }
            m_out_l.push_back(ret);
            break;
    }

    if (m_field) {
        m_type_l.push_back(m_field->getDataType());

        // Track whether the next deref will be a pointer
        m_isRef = vsc::dm::TaskIsTypeFieldRef().eval(m_field);
    } else {
        ret.append("<unknown>");
    }

    DEBUG_LEAVE("visitTypeExprSubField");
}

void GenRefExprExecModel::visitTypeExprVal(vsc::dm::ITypeExprVal *e) {
    DEBUG_ENTER("visitTypeExprVal");
    OutputStr out;
    GenExpr(&out, e);
    m_out_l.push_back(out.getValue());
    DEBUG_LEAVE("visitTypeExprVal");
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

void GenRefExprExecModel::visitTypeFieldRegGroup(arl::dm::ITypeFieldRegGroup *f) {
    m_regRef = true;
}

void GenRefExprExecModel::visitTypeFieldRegGroupArr(arl::dm::ITypeFieldRegGroupArr *f) {
    visitTypeFieldRegGroup(f);
}

void GenRefExprExecModel::init(KindE kind) {
    m_out_l.clear();
    m_type_l.clear();
    m_depth = 0;
    m_depth_ss = 0;
    m_isRef = m_ctxtPtr;
    m_kind = kind;
    m_isFieldRef = false;
    m_isRefCountedField = false;
    m_isRefFieldRef = false;
    m_isAggregateFieldRef = false;
    m_regRef = false;
}

std::string GenRefExprExecModel::strval() {
    std::string ret;
    for (std::vector<std::string>::const_iterator
        it=m_out_l.begin();
        it!=m_out_l.end(); it++) {
        ret.append(*it);
    }
    return ret;
}
void GenRefExprExecModel::GenExpr(IOutput *out, vsc::dm::ITypeExpr *e) {
    GenExpr(out, e, m_kind);
}

void GenRefExprExecModel::GenExpr(IOutput *out, vsc::dm::ITypeExpr *e, KindE kind) {
    DEBUG_ENTER("GenExpr");
    KindE kind_sav = m_kind;
    int32_t depth_sav = m_depth;
    bool regRef_sav = m_regRef;
    m_kind = kind;
    m_depth = 0;
    TaskGenerateExpr(m_gen, this, out).generate(e);
    m_kind = kind_sav;
    m_depth = depth_sav;
    m_regRef = regRef_sav;
    DEBUG_LEAVE("GenExpr");
}

void GenRefExprExecModel::GenExprStandalone(IOutput *out, vsc::dm::ITypeExpr *e) {
    DEBUG_ENTER("GenExprStandalone");
    GenRefExprExecModel genref(m_gen, m_ctxt, m_ctxtRef, m_ctxtPtr, m_bupRef, m_bupPtr);
    TaskGenerateExpr(m_gen, &genref, out).generate(e);
    DEBUG_LEAVE("GenExprStandalone");
}


dmgr::IDebug *GenRefExprExecModel::m_dbg = 0;

}
}
}
}
