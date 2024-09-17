/**
 * IModelCustomGen.h
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
#include "vsc/dm/IAssociatedData.h"
#include "zsp/arl/dm/ITypeExprMethodCallContext.h"
#include "zsp/arl/dm/ITypeExprMethodCallStatic.h"
#include "gen/IOutput.h"
#include "IGenRefExpr.h"

namespace zsp {
namespace sv {
namespace gen {
namespace exec {

class TaskGenerate;

class IModelCustomGen : 
    public virtual vsc::dm::IAssociatedData {
public:

    virtual ~IModelCustomGen() { }

    virtual void genExprMethodCallStaticB(
        TaskGenerate                        *gen,
        IOutput                             *out,
        IGenRefExpr                         *refgen,
        arl::dm::ITypeExprMethodCallStatic  *call) = 0;

    virtual void genExprMethodCallStaticNB(
        TaskGenerate                        *gen,
        IOutput                             *out,
        IGenRefExpr                         *refgen,
        arl::dm::ITypeExprMethodCallStatic  *call) = 0;

    virtual void genExprMethodCallContextB(
        TaskGenerate                        *gen,
        IOutput                             *out,
        IGenRefExpr                         *refgen,
        arl::dm::ITypeExprMethodCallContext *call) = 0;

    virtual void genExprMethodCallContextNB(
        TaskGenerate                        *gen,
        IOutput                             *out,
        IGenRefExpr                         *refgen,
        arl::dm::ITypeExprMethodCallContext *call) = 0;

    virtual void genFwdDecl(
        TaskGenerate                        *gen,
        IOutput                             *out,
        vsc::dm::IDataType                  *type) = 0;

    virtual void genDefinition(
        TaskGenerate                        *gen,
        IOutput                             *out,
        vsc::dm::IDataType                  *type) = 0;

    virtual void genFunctionDefinition(
        TaskGenerate                        *gen,
        IOutput                             *out,
        IGenRefExpr                         *refgen,
        arl::dm::IDataTypeFunction          *func) = 0;

};

} /* namespace exec */
} /* namespace gen */
} /* namespace sv */
} /* namespace zsp */


