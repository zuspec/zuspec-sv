/*
 * TaskBuildActivityInfo.cpp
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
#include "dmgr/impl/DebugMacros.h"
#include "vsc/dm/impl/TaskResolveFieldRefExpr.h"
#include "TaskBuildActivityInfo.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskBuildActivityInfo::TaskBuildActivityInfo(
    dmgr::IDebugMgr         *dmgr,
    INameMap                *name_m) : m_dmgr(dmgr), m_name_m(name_m) {
    DEBUG_INIT("zsp::sv::gen::exec::TaskBuildActivityInfo", dmgr);
}

TaskBuildActivityInfo::~TaskBuildActivityInfo() {

}

std::vector<ActivityInfoUP> TaskBuildActivityInfo::build(
    arl::dm::IDataTypeAction    *action_t,
    arl::dm::IDataTypeActivity  *root) {
    DEBUG_ENTER("build %p", action_t);
    std::vector<ActivityInfoUP> ret;
    m_info = &ret;
    m_type_m.clear();
    m_depth = 0;
    m_action_s.push_back(action_t);
    root->accept(m_this);
    m_action_s.pop_back();
    DEBUG_LEAVE("build (%d)", ret.size());
    return ret;
}

void TaskBuildActivityInfo::visitDataTypeActivity(arl::dm::IDataTypeActivity *t) {
    DEBUG_ENTER("visitDataTypeActivity");
    DEBUG_LEAVE("visitDataTypeActivity");
}

void TaskBuildActivityInfo::visitDataTypeActivitySequence(arl::dm::IDataTypeActivitySequence *t) {
    DEBUG_ENTER("visitDataTypeActivitySequence %p", t);
    if (!m_depth) {
        std::pair<ActivityVariant *, bool> variant = getVariant(t);

        m_variant_s.push_back(variant.first);
    }

    m_activity_s.push_back(t);
    m_depth++;
    for (uint32_t i=0; i<t->getActivities().size(); i++) {
        m_ctxt.pushPath(i);
        t->getActivities().at(i)->accept(m_this);
        m_ctxt.popPath();
    }
    m_depth--;
    m_activity_s.pop_back();

    if (!m_depth) {
        m_variant_s.pop_back();
    }
    DEBUG_LEAVE("visitDataTypeActivitySequence");
}

void TaskBuildActivityInfo::visitDataTypeActivityTraverse(arl::dm::IDataTypeActivityTraverse *t) {
    DEBUG_ENTER("visitDataTypeActivityTraverse");
    // TODO: Navigate to the field so we can get the type
    t->getTarget()->accept(m_this);

    // Should now have the action type
    DEBUG_LEAVE("visitDataTypeActivityTraverse");
}

void TaskBuildActivityInfo::visitDataTypeActivityTraverseType(arl::dm::IDataTypeActivityTraverseType *t) {
    DEBUG_ENTER("visitDataTypeActivityTraverseType");
    if (t->getTarget()->activities().size()) {
        // TODO: Really want a single activity

        arl::dm::IDataTypeActivity *activity = 
            t->getTarget()->activities().at(0)->getDataTypeT<arl::dm::IDataTypeActivity>();
        m_action_s.push_back(t->getTarget());
        std::pair<ActivityVariant *, bool> variant = getVariant(activity);

        // Ensure that the parent variant uses *this* type permutation
        m_variant_s.back()->mapVariant(activity, variant.first);

        if (variant.second) {
            m_variant_s.push_back(variant.first);
            activity->accept(m_this);
            m_variant_s.pop_back();
        }
        m_action_s.pop_back();
    }
    DEBUG_LEAVE("visitDataTypeActivityTraverseType");
}

std::pair<ActivityVariant *, bool> TaskBuildActivityInfo::getVariant(arl::dm::IDataTypeActivity *t) {
    std::pair<ActivityVariant *, bool> ret = {0, false};
    std::map<arl::dm::IDataTypeActivity *, ActivityInfo *>::const_iterator it;
    DEBUG_ENTER("getVariant %d %p", 
        m_action_s.size(),
        m_action_s.size()?m_action_s.back():0);

    if ((it=m_type_m.find(t)) == m_type_m.end()) {
        ActivityInfo *info = new ActivityInfo(
            m_action_s.size()?m_action_s.back():0,
            t);
        ret.first = new ActivityVariant(m_name_m->getName(t), info, m_ctxt);
        ret.second = true;
        info->addVariant(ret.first);
        m_type_m.insert({t, info}).first;
        m_info->push_back(ActivityInfoUP(info));
    } else {
        // Check to see if this specific variant is represented
        for (std::vector<ActivityVariantUP>::const_iterator
            v_it=it->second->variants().begin();
            v_it!=it->second->variants().end() && !ret.first; v_it++) {
            if ((*v_it)->ctxt().equals(m_ctxt)) {
                ret.first = v_it->get();
                ret.second = false;
            }
        }

        if (ret.first) {
            // Didn't find this particular variant
            const std::string &name = m_name_m->getName(t);
            char *tmp = (char *)alloca(name.size()+64);
            sprintf(tmp, "%s_%d", name.c_str(), (int)it->second->variants().size());
            ret.first = new ActivityVariant(tmp, it->second, m_ctxt);
            it->second->addVariant(ret.first);
            ret.second = true;
        }
    }

    DEBUG_LEAVE("getVariant %p %d", ret.first, ret.second);
    return ret;
}

dmgr::IDebug *TaskBuildActivityInfo::m_dbg = 0;

}
}
}
}
