/*
 * Actor.cpp
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
#include "Actor.h"
#include "vsc/solvers/FactoryExt.h"
#include "zsp/arl/eval/FactoryExt.h"

typedef void *chandle;

namespace zsp {
namespace sv {


Actor::Actor(
        arl::dm::IContext               *ctxt,
        const std::string               &seed,
        arl::dm::IDataTypeComponent     *comp_t,
        arl::dm::IDataTypeAction        *action_t,
        arl::eval::IEvalBackend         *backend) {
    arl::eval::IFactory *eval_f = zsp_arl_eval_getFactory();
    vsc::solvers::IFactory *solvers_f = vsc_solvers_getFactory();

    m_randstate = vsc::solvers::IRandStateUP(solvers_f->mkRandState(seed));

    m_evalCtxt = arl::eval::IEvalContextUP(
        eval_f->mkEvalContextFullElab(
            vsc_solvers_getFactory(),
            ctxt,
            m_randstate.get(),
            0, // TODO: pyeval
            comp_t,
            action_t,
            backend));

    for (std::vector<arl::dm::IDataTypeFunction *>::const_iterator
        it=m_evalCtxt->getSolveFunctions().begin();
        it!=m_evalCtxt->getSolveFunctions().end(); it++) {
        m_func_m.insert({(*it)->name(), *it});
    }
    for (std::vector<arl::dm::IDataTypeFunction *>::const_iterator
        it=m_evalCtxt->getTargetFunctions().begin();
        it!=m_evalCtxt->getTargetFunctions().end(); it++) {
        m_func_m.insert({(*it)->name(), *it});
    }

}

Actor::~Actor() {

}

int32_t Actor::eval() {
    return m_evalCtxt->eval();
}

bool Actor::registerFunctionId(const std::string &name, int32_t id) {
    std::map<std::string, arl::dm::IDataTypeFunction *>::const_iterator it;

    it = m_func_m.find(name);

    if (it != m_func_m.end()) {
        m_func_id_m.insert({it->second, id});
        return true;
    } else {
        return false;
    }
}

int32_t Actor::getFunctionId(arl::dm::IDataTypeFunction *f) {
    std::map<arl::dm::IDataTypeFunction *, int32_t>::const_iterator it;

    it = m_func_id_m.find(f);

    if (it != m_func_id_m.end()) {
        return it->second;
    } else {
        return -1;
    }
}


}
}

