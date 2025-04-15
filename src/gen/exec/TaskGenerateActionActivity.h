/**
 * TaskGenerateActionActivity.h
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

class TaskGenerateActionActivity :   
    public virtual arl::dm::VisitorBase {
public:
    TaskGenerateActionActivity(
        TaskGenerate                *gen,   
        IGenRefExpr                 *genref,
        IOutput                     *out,
        vsc::dm::IDataTypeStruct    *t
    );

    virtual ~TaskGenerateActionActivity();

    virtual void generate(const std::vector<arl::dm::ITypeFieldActivityUP> &activities);

    virtual void visitDataTypeActivityParallel(arl::dm::IDataTypeActivityParallel *t) override;

    virtual void visitDataTypeActivitySequence(arl::dm::IDataTypeActivitySequence *t) override;

    virtual void visitDataTypeActivityTraverse(arl::dm::IDataTypeActivityTraverse *t) override;

    virtual void visitDataTypeActivityTraverseType(arl::dm::IDataTypeActivityTraverseType *t) override;

private:
    enum ScopeT {
        Sequence,
        Parallel,
        Schedule
    };

private:
    void enter_activity(ScopeT kind);

    void leave_activity();

    IOutput *out() const { return m_out_s.back(); }

    ScopeT scope_t() const {
        return (m_scope_s.size())?m_scope_s.back():ScopeT::Sequence;
    }

private:
    dmgr::IDebug                    *m_dbg;
    TaskGenerate                    *m_gen;
    IGenRefExpr                     *m_genref;
    arl::dm::IDataTypeAction        *m_action_t;
    OutputStrUP                     m_out_case;
    OutputActivityScope             *m_out_activity;
    IOutput                         *m_out_top;
    int32_t                         m_id;
    std::vector<ScopeT>             m_scope_s;
    std::vector<OutputStrUP>        m_out_l;
    std::vector<IOutput *>          m_out_s;
    IOutput                         *m_out;
    int32_t                         m_depth;
    std::vector<ActivityVariant *>  m_variant_s;

};

}
}
}
}


