/**
 * ActivityVariant.h
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
#include <string>
#include <vector>
#include "ActivityContext.h"
#include "zsp/arl/dm/IDataTypeActivity.h"

namespace zsp {
namespace sv {
namespace gen {
namespace exec {

class ActivityInfo;

class ActivityVariant;
using ActivityVariantUP=vsc::dm::UP<ActivityVariant>;
class ActivityVariant {
public:
    ActivityVariant(
        const std::string       &name,
        ActivityInfo            *info,
        const ActivityContext   &ctxt);

    virtual ~ActivityVariant();

    const std::string &name() const { return m_name; }

    bool equals(const std::vector<int32_t> &other) const;

    ActivityInfo *info() const {
        return m_info;
    }

    const ActivityContext &ctxt() const {
        return m_ctxt;
    }

    ActivityVariant *getVariant(arl::dm::IDataTypeActivity *t);

    void mapVariant(arl::dm::IDataTypeActivity *t, ActivityVariant *v);

protected:
    std::string                                                 m_name;
    ActivityInfo                                                *m_info;
    ActivityContext                                             m_ctxt;
    std::map<arl::dm::IDataTypeActivity *, ActivityVariant *>   m_activity_m;

};

}
}
}
}


