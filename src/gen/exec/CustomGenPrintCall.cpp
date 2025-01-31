/*
 * CustomGenPrintCall.cpp
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
#include "vsc/dm/ITypeExprVal.h"
#include "vsc/dm/impl/ValRefStr.h"
#include "TaskGenerateExpr.h"
#include "CustomGenPrintCall.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


CustomGenPrintCall::CustomGenPrintCall(
    dmgr::IDebugMgr                     *dmgr) : CustomGenBase(dmgr) {
    DEBUG_INIT("zsp::sv::gen::exec::CustomGenPrintCall", dmgr);
}

CustomGenPrintCall::~CustomGenPrintCall() {

}

void CustomGenPrintCall::genExprMethodCallContext(
        TaskGenerate                        *gen,
        IOutput                             *out,
        IGenRefExpr                         *refgen,
        arl::dm::ITypeExprMethodCallContext *call) {
    int32_t cast_idx = 0;
    DEBUG_ENTER("genExprMethodCallContext");


    DEBUG_LEAVE("genExprMethodCallContext");
}

void CustomGenPrintCall::genExprMethodCallStatic(
        TaskGenerate               *gen,
        IOutput                             *out,
        IGenRefExpr                         *refgen,
        arl::dm::ITypeExprMethodCallStatic  *call) {
    DEBUG_ENTER("genExprMethodCallStatic");
    std::string fmt = "<error>";

    vsc::dm::ITypeExprVal *fmt_e = dynamic_cast<vsc::dm::ITypeExprVal *>(
        call->getParameters().at(0).get());
    if (fmt_e) {
        vsc::dm::ValRefStr fmt_v(fmt_e->val());
        fmt = fmt_v.val();
    } else {
        DEBUG_ERROR("Format expression isn't a literal");
    }

    int32_t last_c = -1;
    for (int32_t i=0; i<fmt.size(); i++) {
        if (fmt[i] == '%' && last_c != '%') {
            if (i+1 < fmt.size() && 
                fmt[i+1] != '%' && fmt[i+1] != '-' && 
                !(fmt[i+1] >= '0' && fmt[i+1] <= '9')) {
                fmt.insert(i+1, "0");
            }
        }
        last_c = fmt[i];
    }

    out->write("`zsp_print(exec_b, (\"%s\"", fmt.c_str());

    if (call->getParameters().size() > 1) {
        for (std::vector<vsc::dm::ITypeExprUP>::const_iterator
            it=call->getParameters().begin()+1;
            it!=call->getParameters().end(); it++) {
            out->write(", ");
            TaskGenerateExpr(gen, refgen, out).generate(it->get());
        }
    }

    out->write("))");
    DEBUG_LEAVE("genExprMethodCallStatic");
}


}
}
}
}
