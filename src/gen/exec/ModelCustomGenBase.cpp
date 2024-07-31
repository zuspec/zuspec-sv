/*
 * ModelCustomGenBase.cpp
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
//#include "ModelFwdDecl.h"
#include "ModelCustomGenBase.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


ModelCustomGenBase::ModelCustomGenBase(
    dmgr::IDebugMgr         *dmgr) : m_dbg(0) {

}

ModelCustomGenBase::~ModelCustomGenBase() {

}

void ModelCustomGenBase::genExprMethodCallStaticB(
        TaskGenerate               *gen,
        IOutput                             *out,
        IGenRefExpr                         *refgen,
        arl::dm::ITypeExprMethodCallStatic  *call) { }

void ModelCustomGenBase::genExprMethodCallStaticNB(
        TaskGenerate               *gen,
        IOutput                             *out,
        IGenRefExpr                         *refgen,
        arl::dm::ITypeExprMethodCallStatic  *call) { }

void ModelCustomGenBase::genExprMethodCallContextB(
        TaskGenerate               *gen,
        IOutput                             *out,
        IGenRefExpr                         *refgen,
        arl::dm::ITypeExprMethodCallContext *call) { }

void ModelCustomGenBase::genExprMethodCallContextNB(
        TaskGenerate               *gen,
        IOutput                             *out,
        IGenRefExpr                         *refgen,
        arl::dm::ITypeExprMethodCallContext *call) { }
        
void ModelCustomGenBase::genFwdDecl(
        TaskGenerate               *gen,
        IOutput                             *out,
        vsc::dm::IDataType                  *type) {
//    ModelFwdDecl(gen, out).generate_dflt(type);
}

void ModelCustomGenBase::genDefinition(
        TaskGenerate                        *gen,
        IOutput                             *out,
        vsc::dm::IDataType                  *type) {

}

}
}
}
}
