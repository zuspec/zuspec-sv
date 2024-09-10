/**
 * TaskBuildTypeCollection.h
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
#include "zsp/arl/dm/IContext.h"
#include "zsp/arl/dm/IDataTypeAction.h"
#include "zsp/arl/dm/IDataTypeComponent.h"
#include "zsp/arl/dm/impl/VisitorBase.h"
#include "TypeCollection.h"

namespace zsp {
namespace sv {
namespace gen {



class TaskBuildTypeCollection : public virtual arl::dm::VisitorBase {
public:
    TaskBuildTypeCollection(
        dmgr::IDebugMgr     *dmgr
    );

    virtual ~TaskBuildTypeCollection();

    TypeCollectionUP build(arl::dm::IContext *ctxt);

    TypeCollectionUP build(
        arl::dm::IDataTypeComponent     *comp_t,
        arl::dm::IDataTypeAction        *action_t);
        
	virtual void visitDataTypeAction(arl::dm::IDataTypeAction *i) override;
        
	virtual void visitDataTypeActivityParallel(arl::dm::IDataTypeActivityParallel *t) override;

	virtual void visitDataTypeActivitySequence(arl::dm::IDataTypeActivitySequence *t) override;

	virtual void visitDataTypeActivityTraverse(arl::dm::IDataTypeActivityTraverse *t) override;

	virtual void visitDataTypeActivityTraverseType(arl::dm::IDataTypeActivityTraverseType *t) override;

    virtual void visitDataTypeAddrClaim(arl::dm::IDataTypeAddrClaim *t) override;

    virtual void visitDataTypeAddrSpaceTransparentC(arl::dm::IDataTypeAddrSpaceTransparentC *t) override;

	virtual void visitDataTypeComponent(arl::dm::IDataTypeComponent *t) override;

	virtual void visitDataTypeFlowObj(arl::dm::IDataTypeFlowObj *t) override;

	virtual void visitDataTypePackedStruct(arl::dm::IDataTypePackedStruct *t) override;

	virtual void visitDataTypeStruct(vsc::dm::IDataTypeStruct *t) override;

    virtual void visitTypeField(vsc::dm::ITypeField *t) override;

    virtual void visitTypeFieldPhy(vsc::dm::ITypeFieldPhy *t) override;

    virtual void visitTypeFieldRef(vsc::dm::ITypeFieldRef *t) override;

	virtual void visitTypeProcStmtVarDecl(arl::dm::ITypeProcStmtVarDecl *s) override;

protected:
    void process_exec_blocks(arl::dm::IDataTypeArlStruct *t);

protected:
    enum class Type {
        Action,
        Component,
        Field,
        FlowObj,
        Struct
    };

protected:
    static dmgr::IDebug                     *m_dbg;
    TypeCollectionUP                        m_type_c;
    std::vector<vsc::dm::IDataType *>       m_dtype_s;
    std::vector<Type>                       m_type_s;

};

}
}
}


