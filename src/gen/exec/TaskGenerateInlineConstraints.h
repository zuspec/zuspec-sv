/**
 * TaskGenerateInlineConstraints.h
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

class TaskGenerateInlineConstraints :
    public arl::dm::VisitorBase {
public:
    TaskGenerateInlineConstraints(
        TaskGenerateActorPkgPrv *gen,
        IGenRefExpr             *genref,
        IOutput                 *out
    );

    virtual ~TaskGenerateInlineConstraints();
    
    void generate(arl::dm::IDataTypeActivity *activity);

    virtual void visitDataTypeActivityTraverseType(arl::dm::IDataTypeActivityTraverseType *t) override;

private:
    static dmgr::IDebug             *m_dbg;
    TaskGenerateActorPkgPrv         *m_gen;
    IGenRefExpr                     *m_genref;
    IOutput                         *m_out;

};

}
}
}
}


