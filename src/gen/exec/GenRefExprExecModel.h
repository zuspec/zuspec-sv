/**
 * GenRefExprExecModel.h
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
#include "IGenRefExpr.h"

namespace zsp {
namespace sv {
namespace gen {
namespace exec {

class TaskGenerate;


class GenRefExprExecModel : 
    public virtual IGenRefExpr,
    public virtual arl::dm::VisitorBase {
public:
    GenRefExprExecModel(
        TaskGenerate                *gen,
        vsc::dm::IDataTypeStruct    *ctxt,
        const std::string           &ctxtRef,
        bool                        ctxtPtr,
        const std::string           &bupRef="",
        bool                        bupPtr=false
    );

    virtual ~GenRefExprExecModel();

    virtual std::string genLval(vsc::dm::ITypeExpr *ref) override;

    virtual std::string genRval(vsc::dm::ITypeExpr *ref) override;

    virtual std::string genRegAddr(vsc::dm::ITypeExpr *ref) override;

    virtual bool isFieldRefExpr(vsc::dm::ITypeExpr *ref) override;

    virtual bool isRefFieldRefExpr(vsc::dm::ITypeExpr *ref) override;

    virtual bool isAggregateFieldRefExpr(vsc::dm::ITypeExpr *ref) override;

    virtual ResT isRefCountedField(vsc::dm::IAccept *ref) override;

    virtual void pushInline(vsc::dm::IDataTypeStruct *t) override {
        m_inline_s.push_back(t);
    }

    virtual void popInline() override {
        m_inline_s.pop_back();
    }
    
    virtual void pushScope(vsc::dm::ITypeVarScope *s) override {
        m_scope_s.push_back(s);
    }

    virtual void popScope() override {
        m_scope_s.pop_back();
    }

	virtual void visitDataTypeAction(arl::dm::IDataTypeAction *t) override { }

	virtual void visitDataTypeAddrClaim(arl::dm::IDataTypeAddrClaim *t) override;

	virtual void visitDataTypeAddrHandle(arl::dm::IDataTypeAddrHandle *t) override;

    virtual void visitDataTypeArray(vsc::dm::IDataTypeArray *t) override;

	virtual void visitDataTypeComponent(arl::dm::IDataTypeComponent *t) override { }

	virtual void visitDataTypeFlowObj(arl::dm::IDataTypeFlowObj *t) override;

	virtual void visitDataTypeStruct(vsc::dm::IDataTypeStruct *t) override;

    virtual void visitTypeExprArrIndex(vsc::dm::ITypeExprArrIndex *e) override;

    virtual void visitTypeExprMethodCallContext(arl::dm:: ITypeExprMethodCallContext *e) override;

    virtual void visitTypeExprMethodCallStatic(arl::dm::ITypeExprMethodCallStatic *e) override;

	virtual void visitTypeExprRefBottomUp(vsc::dm::ITypeExprRefBottomUp *e) override;

	virtual void visitTypeExprRefInline(vsc::dm::ITypeExprRefInline *e) override;

	virtual void visitTypeExprRefPath(vsc::dm::ITypeExprRefPath *e) override;

	virtual void visitTypeExprRefTopDown(vsc::dm::ITypeExprRefTopDown *e) override;

	virtual void visitTypeExprSubField(vsc::dm::ITypeExprSubField *e) override;

    virtual void visitTypeExprVal(vsc::dm::ITypeExprVal *e) override;

	virtual void visitTypeField(vsc::dm::ITypeField *f) override;

	virtual void visitTypeFieldPhy(vsc::dm::ITypeFieldPhy *f) override;

	virtual void visitTypeFieldRef(vsc::dm::ITypeFieldRef *f) override;

    virtual void visitTypeFieldRegGroup(arl::dm::ITypeFieldRegGroup *f) override;

    virtual void visitTypeFieldRegGroupArr(arl::dm::ITypeFieldRegGroupArr *f) override;

private:    
    enum KindE {
        Lval,
        Rval,
        RegAddr,
        Check
    };

private:
    void init(KindE kind);

    std::string strval();


private:
    static dmgr::IDebug                             *m_dbg;
    KindE                                           m_kind;
    TaskGenerate                                    *m_gen;
    vsc::dm::IDataTypeStruct                        *m_ctxt;
    std::string                                     m_ctxtRef;
    bool                                            m_ctxtPtr;
    std::string                                     m_bupRef;
    bool                                            m_bupPtr;
    std::vector<std::string>                        m_out_l;
    std::vector<vsc::dm::IDataType *>               m_type_l;
    vsc::dm::ITypeField                             *m_field;
    int32_t                                         m_depth;
    int32_t                                         m_depth_ss;
    bool                                            m_regRef;
    bool                                            m_isRef;
    bool                                            m_isFieldRef;
    bool                                            m_isRefFieldRef;
    bool                                            m_isRefCountedField;
    bool                                            m_isAggregateFieldRef;
    std::vector<vsc::dm::IDataTypeStruct *>         m_inline_s;
    std::vector<vsc::dm::ITypeVarScope *>           m_scope_s;

};

}
}
}
}


