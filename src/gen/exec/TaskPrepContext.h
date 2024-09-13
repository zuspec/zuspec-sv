/**
 * TaskPrepContext.h
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
#include "dmgr/IDebugMgr.h"
#include "zsp/arl/dm/IContext.h"
#include "zsp/arl/dm/impl/VisitorBase.h"
#include "zsp/arl/eval/IFactory.h"

namespace zsp {
namespace sv {
namespace gen {
namespace exec {



class TaskPrepContext :
    public virtual arl::dm::VisitorBase {
public:
    TaskPrepContext(
        dmgr::IDebugMgr     *dmgr,
        arl::dm::IContext   *ctxt,
        arl::eval::IFactory *eval_f
    );

    virtual ~TaskPrepContext();

    virtual void prepare();

    virtual void visitDataTypeAction(arl::dm::IDataTypeAction *t) override;

	virtual void visitTypeExecProc(arl::dm::ITypeExecProc *e) override;

private:
    static dmgr::IDebug             *m_dbg;
    dmgr::IDebugMgr                 *m_dmgr;
    arl::dm::IContext               *m_ctxt;
    arl::eval::IFactory             *m_eval_f;

};

}
}
}
}


