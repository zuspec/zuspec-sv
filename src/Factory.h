/**
 * Factory.h
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
#include <memory>
#include "zsp/sv/IFactory.h"

namespace zsp {
namespace sv {

class Factory;
using FactoryUP=std::unique_ptr<Factory>;
class Factory : public virtual IFactory {
public:
    Factory();

    virtual ~Factory();

    virtual void init(dmgr::IDebugMgr *dmgr) override;

    virtual void prepContextExec(
        arl::dm::IContext               *ctxt,
        arl::eval::IFactory             *eval_f) override;

    virtual gen::ITaskGenerate *mkGenerateActorPkg(
        arl::dm::IContext               *ctxt,
        arl::eval::IFactory             *eval_f,
        arl::dm::IDataTypeComponent     *comp_t,
        arl::dm::IDataTypeAction        *action_t,
        std::ostream                    *out) override;

    virtual gen::ITaskGenerate *mkGenerateActorPkgPrv(
        arl::dm::IContext               *ctxt,
        arl::eval::IFactory             *eval_f,
        arl::dm::IDataTypeComponent     *comp_t,
        arl::dm::IDataTypeAction        *action_t,
        std::ostream                    *out) override;

    virtual gen::ITaskGenerate *mkGenerateTypesPkg(
        arl::dm::IContext               *ctxt,
        arl::eval::IFactory             *eval_f,
        std::ostream                    *out) override;


    static Factory *inst();

private:
    static FactoryUP                m_inst;
    dmgr::IDebugMgr                 *m_dmgr;

};

}
}


