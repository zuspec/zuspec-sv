/*
 * TaskBuildTypeCollection.cpp
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
#include "TaskBuildTypeCollection.h"


namespace zsp {
namespace sv {
namespace gen {


TaskBuildTypeCollection::TaskBuildTypeCollection(
    dmgr::IDebugMgr         *dmgr) {
    DEBUG_INIT("zsp::be::sw::TaskBuildTypeCollection", dmgr);
}

TaskBuildTypeCollection::~TaskBuildTypeCollection() {

}

TypeCollectionUP TaskBuildTypeCollection::build(arl::dm::IContext *ctxt) {
    DEBUG_ENTER("build");
    m_type_c = TypeCollectionUP(new TypeCollection());

    for (std::vector<vsc::dm::IDataTypeStructUP>::const_iterator
        it=ctxt->getDataTypeStructs().begin();
        it!=ctxt->getDataTypeStructs().end(); it++) {
        (*it)->accept(m_this);
    }

    DEBUG_LEAVE("build");
    return std::move(m_type_c);
}

TypeCollectionUP TaskBuildTypeCollection::build(
        arl::dm::IDataTypeComponent     *comp_t,
        arl::dm::IDataTypeAction        *action_t) {
    DEBUG_ENTER("build");
    m_type_c = TypeCollectionUP(new TypeCollection());

    comp_t->accept(m_this);
    action_t->accept(m_this);

    DEBUG_LEAVE("build");
    return std::move(m_type_c);
}

void TaskBuildTypeCollection::visitDataTypeAction(arl::dm::IDataTypeAction *t) { 
    DEBUG_ENTER("visitDataTypeAction %s", t->name().c_str());
    m_type_c->addType(t);

    for (int32_t i=m_type_s.size()-1; i>=0; i--) {
        if (m_type_s.at(i) != Type::Field) {
            m_type_c->addDep(
                t,
                m_dtype_s.at(i));
        }
    }

    m_dtype_s.push_back(t);
    m_type_s.push_back(Type::Action);

    for (std::vector<vsc::dm::ITypeFieldUP>::const_iterator
        it=t->getFields().begin();
        it!=t->getFields().end(); it++) {
        (*it)->accept(m_this);
    }

    process_exec_blocks(t);

    for (std::vector<arl::dm::ITypeFieldActivityUP>::const_iterator
        it=t->activities().begin();
        it!=t->activities().end(); it++) {
        (*it)->accept(m_this);
    }

    m_dtype_s.pop_back();
    m_type_s.pop_back();
    DEBUG_LEAVE("visitDataTypeAction");
}
        
void TaskBuildTypeCollection::visitDataTypeActivityParallel(arl::dm::IDataTypeActivityParallel *t) { 

}

void TaskBuildTypeCollection::visitDataTypeActivitySequence(arl::dm::IDataTypeActivitySequence *t) {
    DEBUG_ENTER("visitDataTypeActivitySequence");
    for (std::vector<vsc::dm::ITypeFieldUP>::const_iterator
        it=t->getFields().begin();
        it!=t->getFields().end(); it++) {
        (*it)->accept(m_this);
    }
    for (std::vector<arl::dm::ITypeFieldActivityUP>::const_iterator
        it=t->getActivities().begin();
        it!=t->getActivities().end(); it++) {
        (*it)->getDataType()->accept(m_this);
    }
    DEBUG_LEAVE("visitDataTypeActivitySequence");
}

void TaskBuildTypeCollection::visitDataTypeActivityTraverse(arl::dm::IDataTypeActivityTraverse *t) {
    DEBUG_ENTER("visitDataTypeActivityTraverse");

    DEBUG_LEAVE("visitDataTypeActivityTraverse");
}

void TaskBuildTypeCollection::visitDataTypeActivityTraverseType(arl::dm::IDataTypeActivityTraverseType *t) {
    DEBUG_ENTER("visitDataTypeActivityTraverseType");
    t->getTarget()->accept(m_this);
    DEBUG_LEAVE("visitDataTypeActivityTraverseType");
}

void TaskBuildTypeCollection::visitDataTypeAddrClaim(arl::dm::IDataTypeAddrClaim *t) {
    DEBUG_ENTER("visitDataTypeAddrClaim");
    m_type_c->addType(t);

    for (int32_t i=m_type_s.size()-1; i>=0; i--) {
        if (m_type_s.at(i) != Type::Field) {
            m_type_c->addDep(
                t,
                m_dtype_s.at(i));
        }
    }

    m_dtype_s.push_back(t);
    m_type_s.push_back(Type::Action);

    for (std::vector<vsc::dm::ITypeFieldUP>::const_iterator
        it=t->getFields().begin();
        it!=t->getFields().end(); it++) {
        (*it)->accept(m_this);
    }

    process_exec_blocks(t);

    m_dtype_s.pop_back();
    m_type_s.pop_back();
    DEBUG_LEAVE("visitDataTypeAddrClaim");
}

void TaskBuildTypeCollection::visitDataTypeAddrSpaceTransparentC(arl::dm::IDataTypeAddrSpaceTransparentC *t) {
    DEBUG_ENTER("visitDataTypeAddrSpaceTransparentC");
    if (m_type_c->addType(t)) {
        for (int32_t i=m_type_s.size()-1; i>=0; i--) {
            if (m_type_s.at(i) != Type::Field) {
                m_type_c->addDep(
                    t,
                    m_dtype_s.at(i));
            }
        }
    }
    DEBUG_LEAVE("visitDataTypeAddrSpaceTransparentC");
}

void TaskBuildTypeCollection::visitDataTypeComponent(arl::dm::IDataTypeComponent *t) { 
    DEBUG_ENTER("visitDataTypeComponent");

    m_type_c->addType(t);

    for (int32_t i=m_type_s.size()-1; i>=0; i--) {
        if (m_type_s.at(i) != Type::Field) {
            m_type_c->addDep(
                t,
                m_dtype_s.at(i));
        }
    }

    m_dtype_s.push_back(t);
    m_type_s.push_back(Type::Component);

    for (std::vector<vsc::dm::ITypeFieldUP>::const_iterator
        it=t->getFields().begin();
        it!=t->getFields().end(); it++) {
        (*it)->accept(m_this);
    }

    process_exec_blocks(t);

    m_type_s.pop_back();
    m_dtype_s.pop_back();
    DEBUG_LEAVE("visitDataTypeComponent");
}

void TaskBuildTypeCollection::visitDataTypeFlowObj(arl::dm::IDataTypeFlowObj *t) {
    // Flow objects are always references
    // We need to generate a representation for this type, but don't record deps
    m_type_c->addType(t);
}

void TaskBuildTypeCollection::visitDataTypePackedStruct(arl::dm::IDataTypePackedStruct *t) {
    DEBUG_ENTER("visitDataTypePackedStruct");
    m_type_c->addType(t);

    for (int32_t i=m_type_s.size()-1; i>=0; i--) {
        if (m_type_s.at(i) != Type::Field) {
            m_type_c->addDep(
                t,
                m_dtype_s.at(i));
        }
    }

    m_dtype_s.push_back(t);
    m_type_s.push_back(Type::Struct);

    for (std::vector<vsc::dm::ITypeFieldUP>::const_iterator
        it=t->getFields().begin();
        it!=t->getFields().end(); it++) {
        (*it)->accept(m_this);
    }

//    process_exec_blocks(t);

    m_type_s.pop_back();
    m_dtype_s.pop_back();
    DEBUG_LEAVE("visitDataTypePackedStruct");
}

void TaskBuildTypeCollection::visitDataTypeStruct(vsc::dm::IDataTypeStruct *t) { 
    DEBUG_ENTER("visitDataTypeStruct %s", t->name().c_str());
    m_type_c->addType(t);

    for (int32_t i=m_type_s.size()-1; i>=0; i--) {
        if (m_type_s.at(i) != Type::Field) {
            m_type_c->addDep(
                t,
                m_dtype_s.at(i));
        }
    }

    m_dtype_s.push_back(t);
    m_type_s.push_back(Type::Struct);

    for (std::vector<vsc::dm::ITypeFieldUP>::const_iterator
        it=t->getFields().begin();
        it!=t->getFields().end(); it++) {
        (*it)->accept(m_this);
    }

    m_type_s.pop_back();
    m_dtype_s.pop_back();
    DEBUG_LEAVE("visitDataTypeStruct");
}

void TaskBuildTypeCollection::visitTypeField(vsc::dm::ITypeField *t) {
    DEBUG_ENTER("visitTypeField");
    m_type_s.push_back(Type::Field);
    m_dtype_s.push_back(0);
    t->getDataType()->accept(m_this);
    m_dtype_s.pop_back();
    m_type_s.pop_back();
    DEBUG_LEAVE("visitTypeField");
}

void TaskBuildTypeCollection::visitTypeFieldPhy(vsc::dm::ITypeFieldPhy *t) {
    DEBUG_ENTER("visitTypeFieldPhy");
    t->getDataType()->accept(m_this);
    DEBUG_LEAVE("visitTypeFieldPhy");
}

void TaskBuildTypeCollection::visitTypeFieldRef(vsc::dm::ITypeFieldRef *t) {
    DEBUG_ENTER("visitTypeFieldRef");
    if (m_type_s.size() && m_type_s.back() == Type::Action && t->name() == "comp") {
        DEBUG("Don't recurse into comp ref");
    } else {
        t->getDataType()->accept(m_this);
    }

    DEBUG_LEAVE("visitTypeFieldRef");
}

void TaskBuildTypeCollection::visitTypeProcStmtVarDecl(arl::dm::ITypeProcStmtVarDecl *s) {
    DEBUG_ENTER("visitTypeProcStmtVarDecl");
    s->getDataType()->accept(m_this);
    DEBUG_LEAVE("visitTypeProcStmtVarDecl");
}

void TaskBuildTypeCollection::process_exec_blocks(arl::dm::IDataTypeArlStruct *t) {
    static const arl::dm::ExecKindT kinds[] = {
        arl::dm::ExecKindT::Body,
        arl::dm::ExecKindT::InitDown,
        arl::dm::ExecKindT::InitUp,
        arl::dm::ExecKindT::PostSolve,
        arl::dm::ExecKindT::PreSolve };
    
    for (uint32_t ki=0; ki<sizeof(kinds)/sizeof(arl::dm::ExecKindT); ki++) {
        const std::vector<arl::dm::ITypeExecUP> &execs = t->getExecs(kinds[ki]);
        for (std::vector<arl::dm::ITypeExecUP>::const_iterator 
            it=execs.begin();
            it!=execs.end(); it++) {
            (*it)->accept(m_this);
        }
    }

}

dmgr::IDebug *TaskBuildTypeCollection::m_dbg = 0;

}
}
}
