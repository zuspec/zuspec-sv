/*
 * ActivityContext.cpp
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
#include "ActivityContext.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {

ActivityContext::ActivityContext() {

}

ActivityContext::ActivityContext(const std::vector<int32_t> &path) :
    m_path(path.begin(), path.end()) {

}

ActivityContext::~ActivityContext() {

}

bool ActivityContext::equals(const ActivityContext *other) const {
    return equals(other->m_path);
}

bool ActivityContext::equals(const ActivityContext &other) const {
    return equals(other.m_path);
}

bool ActivityContext::equals(const std::vector<int32_t> &other) const {
    bool ret = (m_path.size() == other.size());

    if (ret) {
        for (uint32_t i=0; i<m_path.size() && ret; i++) {
            ret=(m_path.at(i) == other.at(i));
        }
    }
    return ret;
}

}
}
}
}
