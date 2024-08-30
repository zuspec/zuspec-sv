/**
 * ActivityInfo.h
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
#include <vector>
#include "vsc/dm/impl/UP.h"
#include "zsp/arl/dm/IDataTypeAction.h"
#include "zsp/arl/dm/IDataTypeActivity.h"

namespace zsp {
namespace sv {
namespace gen {
namespace exec {

class ActivityVariant;
using ActivityVariantUP=vsc::dm::UP<ActivityVariant>;

class ActivityInfo;
using ActivityInfoUP=vsc::dm::UP<ActivityInfo>;
class ActivityInfo {
public:
    ActivityInfo(
        arl::dm::IDataTypeAction   *action,
        arl::dm::IDataTypeActivity *activity);

    virtual ~ActivityInfo();

    arl::dm::IDataTypeAction *action() const {
        return m_action;
    }

    arl::dm::IDataTypeActivity *activity() const {
        return m_activity;
    }

    const std::vector<ActivityVariantUP> &variants() const {
        return m_variants;
    }

    void addVariant(ActivityVariant *variant) {
        m_variants.push_back(ActivityVariantUP(variant));
    }

protected:
    arl::dm::IDataTypeAction            *m_action;
    arl::dm::IDataTypeActivity          *m_activity;
    std::vector<ActivityVariantUP>      m_variants;

};

}
}
}
}


