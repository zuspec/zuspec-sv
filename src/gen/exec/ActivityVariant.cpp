/*
 * ActivityVariant.cpp
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
#include "ActivityInfo.h"
#include "ActivityVariant.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


ActivityVariant::ActivityVariant(
    const std::string       &name,
    ActivityInfo            *info,
    const ActivityContext   &ctxt) : m_name(name), m_info(info), m_ctxt(ctxt) {

}

ActivityVariant::~ActivityVariant() {

}

bool ActivityVariant::equals(const std::vector<int32_t> &other) const {
    return m_ctxt.equals(other);
}

ActivityVariant *ActivityVariant::getVariant(arl::dm::IDataTypeActivity *t) {
    ActivityVariant *ret = 0;
    std::map<arl::dm::IDataTypeActivity *, ActivityVariant *>::const_iterator it
        = m_activity_m.find(t);
    if (it != m_activity_m.end()) {
        ret = it->second;
    }
    return ret;
}

void ActivityVariant::mapVariant(arl::dm::IDataTypeActivity *t, ActivityVariant *v) {
    m_activity_m.insert({t, v});
}

}
}
}
}
