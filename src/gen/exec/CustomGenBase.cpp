/*
 * CustomGenBase.cpp
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
#include "CustomGenBase.h"
#include "TaskGenerate.h"
#include "TaskGenerateExpr.h"
#include "TaskGenerateFunction.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


CustomGenBase::CustomGenBase(
    dmgr::IDebugMgr         *dmgr) : m_dbg(0) {

}

CustomGenBase::~CustomGenBase() {

}

void CustomGenBase::genExprMethodCallStatic(
        TaskGenerate                        *gen,
        IOutput                             *out,
        IGenRefExpr                         *refgen,
        arl::dm::ITypeExprMethodCallStatic  *call) { 
    std::string name = call->getTarget()->name();
    int idx = name.rfind("::");
    if (idx != -1) {
        name = name.substr(idx+2);
    }
    out->write("%s(", name.c_str());
    for (std::vector<vsc::dm::ITypeExprUP>::const_iterator
        it=call->getParameters().begin();
        it!=call->getParameters().end(); it++) {
        if (it != call->getParameters().begin()) {
            out->write(", ");
        }
        TaskGenerateExpr(gen, refgen, out).generate(it->get());
    }
    out->write(")");
}

void CustomGenBase::genExprMethodCallContext(
        TaskGenerate                        *gen,
        IOutput                             *out,
        IGenRefExpr                         *refgen,
        arl::dm::ITypeExprMethodCallContext *call) { 
    std::string name = call->getTarget()->name();
    int idx = name.rfind("::");
    if (idx != -1) {
        name = name.substr(idx+2);
    }
    out->write("%s.%s(",
        refgen->genRval(call->getContext()).c_str(),
        name.c_str());
    for (std::vector<vsc::dm::ITypeExprUP>::const_iterator
        it=call->getParameters().begin();
        it!=call->getParameters().end(); it++) {
        if (it != call->getParameters().begin()) {
            out->write(", ");
        }
        TaskGenerateExpr(gen, refgen, out).generate(it->get());
    }
    out->write(")");
 }

void CustomGenBase::genDefinition(
        TaskGenerate                        *gen,
        IOutput                             *out,
        vsc::dm::IDataType                  *type) {

}

void CustomGenBase::genFunctionDefinition(
        TaskGenerate                        *gen,
        IOutput                             *out,
        IGenRefExpr                         *refgen,
        arl::dm::IDataTypeFunction          *func) {
    TaskGenerateFunction(gen, refgen, out).generate(func);
}

}
}
}
}
