/**
 * Actor.h
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
#include <map>
#include "vsc/solvers/IRandState.h"
#include "zsp/arl/dm/IDataTypeAction.h"
#include "zsp/arl/dm/IDataTypeComponent.h"
#include "zsp/arl/eval/IEvalBackend.h"
#include "zsp/arl/eval/IEvalContext.h"

namespace zsp {
namespace sv {



class Actor {
public:
    Actor(
        arl::dm::IContext               *ctxt,
        const std::string               &seed,
        arl::dm::IDataTypeComponent     *comp_t,
        arl::dm::IDataTypeAction        *action_t,
        arl::eval::IEvalBackend         *backend
    );

    virtual ~Actor();

    int32_t eval();

    bool registerFunctionId(const std::string &name, int32_t id);

    int32_t getFunctionId(arl::dm::IDataTypeFunction *f);

private:
    arl::eval::IEvalContextUP                               m_evalCtxt;
    vsc::solvers::IRandStateUP                              m_randstate;
    std::map<std::string,arl::dm::IDataTypeFunction *>      m_func_m;
    std::map<arl::dm::IDataTypeFunction *, int32_t>         m_func_id_m;

};

}
}


