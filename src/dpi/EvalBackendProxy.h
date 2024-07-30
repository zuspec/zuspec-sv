/**
 * EvalBackendProxy.h
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
#include "zsp/arl/eval/impl/EvalBackendBase.h"

namespace zsp {
namespace sv {



class EvalBackendProxy : public virtual arl::eval::EvalBackendBase {
public:
    EvalBackendProxy();

    virtual ~EvalBackendProxy();

    virtual void callFuncReq(
            arl::eval::IEvalThread              *thread,
            arl::dm::IDataTypeFunction          *func_t,
            const std::vector<vsc::dm::ValRef>  &params) override;

    virtual void emitMessage(const std::string &msg) override;

private:
    std::vector<vsc::dm::ValRef>                m_params;

};

}
}


