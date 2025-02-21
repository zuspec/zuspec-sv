/*
 * CustomGenExecFunc.cpp
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
#include "TaskGenerate.h"
#include "TaskGenerateExpr.h"
#include "CustomGenExecFunc.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


CustomGenExecFunc::CustomGenExecFunc(dmgr::IDebugMgr *dmgr) :
    CustomGenBase(dmgr) {

}

CustomGenExecFunc::~CustomGenExecFunc() {

}

void CustomGenExecFunc::genExprMethodCallStatic(
    TaskGenerate                        *gen,
    IOutput                             *out,
    IGenRefExpr                         *refgen,
    arl::dm::ITypeExprMethodCallStatic  *call) {
    arl::dm::IDataTypeFunction *target = call->getTarget();
    std::string name = target->name();
    int colon_idx = name.find("::");
    name = name.substr(colon_idx+2);

    // TODO: this relies on direct read usages being remapped

    out->write("executor.%s(", name.c_str());
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

}
}
}
}
