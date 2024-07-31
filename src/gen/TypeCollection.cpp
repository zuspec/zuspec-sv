/*
 * TypeCollection.cpp
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
#include "vsc/dm/IDataTypeStruct.h"
#include "TypeCollection.h"


namespace zsp {
namespace sv {
namespace gen {


TypeCollection::TypeCollection() {

}

TypeCollection::~TypeCollection() {

}

bool TypeCollection::addType(vsc::dm::IDataType *t) {
    std::unordered_map<vsc::dm::IDataType *, int32_t>::const_iterator it;

    if ((it=m_type_m.find(t)) == m_type_m.end()) {
        int32_t id = m_type_l.size();
        m_type_l.push_back(t);
        m_dep_m.push_back(std::set<int32_t>());
        m_type_m.insert({t, id});
        return true;
    } else {
        return false;
    }
}

int32_t TypeCollection::getTypeId(vsc::dm::IDataType *t) {
    std::unordered_map<vsc::dm::IDataType *, int32_t>::const_iterator it;

    return ((it=m_type_m.find(t)) != m_type_m.end())?it->second:-1;
}

void TypeCollection::addDep(
        vsc::dm::IDataType      *target,
        vsc::dm::IDataType      *dependent) {
    int32_t target_id = getTypeId(target);
    int32_t dependent_id = getTypeId(dependent);

    m_dep_m.at(target_id).insert(dependent_id);
}

vsc::dm::IDataType *TypeCollection::getType(int32_t id) {
    return m_type_l.at(id);
}

std::vector<int32_t> TypeCollection::sort() {
    std::vector<int32_t> sorted;
    std::vector<int32_t> indegree(m_dep_m.size(), 0);

    // Load up the indegree vector
    for (std::vector<std::set<int32_t>>::const_iterator
        it=m_dep_m.begin();
        it!=m_dep_m.end(); it++) {
        for (std::set<int32_t>::const_iterator
            t_it=it->begin();
            t_it!=it->end(); t_it++) {
            indegree[*t_it]++;
        }
    }

    // Next, find root nodes that have no dependencies
    std::vector<int32_t> roots;
    for (int32_t i=0; i<indegree.size(); i++) {
        if (!indegree.at(i)) {
            roots.push_back(i);
        }
    }

    // Now, loop through and sort based on dependencies
    while (roots.size()) {
        int32_t type_id = roots.front();
        roots.erase(roots.begin());

        sorted.push_back(type_id);

        // Adjust dependencies for the type-id we just removed
        for (std::set<int32_t>::const_iterator
            t_it=m_dep_m.at(type_id).begin();
            t_it!=m_dep_m.at(type_id).end(); t_it++) {
            if (indegree.at(*t_it)) {
                indegree.at(*t_it) -= 1;
            }
            if (indegree.at(*t_it) == 0) {
                // This is now a root
                roots.push_back(*t_it);
            }
        }
    }

    return sorted;
}

}
}
}
