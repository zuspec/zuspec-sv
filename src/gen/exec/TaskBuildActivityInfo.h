/**
 * TaskBuildActivityInfo.h
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
#include <vector>
#include "dmgr/IDebugMgr.h"
#include "zsp/arl/dm/impl/VisitorBase.h"
#include "gen/INameMap.h"
#include "ActivityInfo.h"
#include "ActivityVariant.h"

namespace zsp {
namespace sv {
namespace gen {
namespace exec {



class TaskBuildActivityInfo :
    public virtual arl::dm::VisitorBase {
public:
    TaskBuildActivityInfo(
        dmgr::IDebugMgr         *dmgr,
        INameMap                *name_m
    );

    virtual ~TaskBuildActivityInfo();

    std::vector<ActivityInfoUP> build(
        arl::dm::IDataTypeAction    *action_t,
        arl::dm::IDataTypeActivity  *root);

    virtual void visitDataTypeActivity(arl::dm::IDataTypeActivity *t) override;

    virtual void visitDataTypeActivitySequence(arl::dm::IDataTypeActivitySequence *t) override;

    virtual void visitDataTypeActivityTraverse(arl::dm::IDataTypeActivityTraverse *t) override;

    virtual void visitDataTypeActivityTraverseType(arl::dm::IDataTypeActivityTraverseType *t) override;

protected:

    std::pair<ActivityVariant *, bool> getVariant(arl::dm::IDataTypeActivity *t);

private:
    static dmgr::IDebug                                     *m_dbg;
    dmgr::IDebugMgr                                         *m_dmgr;
    INameMap                                                *m_name_m;
    std::vector<ActivityInfoUP>                             *m_info;
    std::map<arl::dm::IDataTypeActivity *, ActivityInfo *>  m_type_m;
    int32_t                                                 m_depth;
    ActivityContext                                         m_ctxt;
    std::vector<arl::dm::IDataTypeAction *>                 m_action_s;
    std::vector<arl::dm::IDataTypeActivityScope *>          m_activity_s;
    std::vector<ActivityVariant *>                          m_variant_s;

};

}
}
}
}


