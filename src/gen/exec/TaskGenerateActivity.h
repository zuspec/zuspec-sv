/**
 * TaskGenerateActivity.h
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
#include "zsp/arl/dm/impl/VisitorBase.h"
#include "IGenRefExpr.h"
#include "gen/IOutput.h"
#include "gen/OutputActivityScope.h"
#include "gen/exec/ActivityVariant.h"

namespace zsp {
namespace sv {
namespace gen {
namespace exec {

class TaskGenerateActorPkgPrv;

class TaskGenerateActivity :   
    public virtual arl::dm::VisitorBase {
public:
    TaskGenerateActivity(
        TaskGenerateActorPkgPrv     *gen,
        IGenRefExpr                 *genref,
        vsc::dm::IDataTypeStruct    *action_t,
        IOutput                     *out
    );

    virtual ~TaskGenerateActivity();

    virtual void generate(ActivityVariant *variant);

    virtual void visitDataTypeActivityBind(arl::dm::IDataTypeActivityBind *t) override;

    virtual void visitDataTypeActivityParallel(arl::dm::IDataTypeActivityParallel *t) override;

    virtual void visitDataTypeActivitySequence(arl::dm::IDataTypeActivitySequence *t) override;

    virtual void visitDataTypeActivityTraverse(arl::dm::IDataTypeActivityTraverse *t) override;

    virtual void visitDataTypeActivityTraverseType(arl::dm::IDataTypeActivityTraverseType *t) override;


private:
    dmgr::IDebug                    *m_dbg;
    TaskGenerateActorPkgPrv         *m_gen;
    IGenRefExpr                     *m_genref;
    vsc::dm::IDataTypeStruct        *m_action_t;
    OutputActivityScope             *m_out_activity;
    IOutput                         *m_out_top;
    IOutput                         *m_out;
    int32_t                         m_depth;
    std::vector<ActivityVariant *>  m_variant_s;

};

}
}
}
}


