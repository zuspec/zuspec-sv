/**
 * TaskGenerateActorPkg.h
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
#include "TaskGenerate.h"

namespace zsp {
namespace sv {
namespace gen {
namespace exec {



class TaskGenerateActorPkg :
    public virtual TaskGenerate {
public:
    TaskGenerateActorPkg(
        dmgr::IDebugMgr                 *dmgr,
        arl::dm::IContext               *ctxt,
        arl::eval::IFactory             *eval_f,
        arl::dm::IDataTypeComponent     *comp_t,
        arl::dm::IDataTypeAction        *action_t,
        std::ostream                    *out);

    virtual ~TaskGenerateActorPkg();

    virtual bool generate() override;

protected:
    arl::dm::IDataTypeComponent         *m_comp_t;
    arl::dm::IDataTypeAction            *m_action_t;

};

}
}
}
}

