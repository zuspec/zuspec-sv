/*
 * OutputExecScope.cpp
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
#include "OutputExecScope.h"


namespace zsp {
namespace sv {
namespace gen {


OutputExecScope::OutputExecScope(
        bool                new_scope,
        const std::string   &ind) : 
        m_new_scope(new_scope), m_decl(ind), m_init(ind), m_exec(ind), m_dtor(ind) {
    if (new_scope) {
        m_decl.inc_ind();
        m_init.inc_ind();
        m_exec.inc_ind();
        m_dtor.inc_ind();
    }
}

OutputExecScope::OutputExecScope(
        bool                new_scope,
        IOutput             *upper) :
        m_new_scope(new_scope), 
        m_decl(upper->ind()), m_init(upper->ind()), m_exec(upper->ind()), m_dtor(upper->ind()) {
    
    // If we'll be creating a new scope, add an additional level of indent
    if (new_scope) {
        m_decl.inc_ind();
        m_init.inc_ind();
        m_exec.inc_ind();
        m_dtor.inc_ind();
    }
}

OutputExecScope::~OutputExecScope() {

}

void OutputExecScope::apply(IOutput *out) {
    if (m_new_scope) {
        out->println("begin");
        out->inc_ind();
    }
    if (m_decl.getValue().size()) {
        out->writes(m_decl.getValue());
        if (m_init.getValue().size()) {
            out->writes("\n");
        }
    }
    if (m_init.getValue().size()) {
        out->writes(m_init.getValue());
        if (m_exec.getValue().size()) {
            out->writes("\n");
        }
    }
    if (m_exec.getValue().size()) {
        out->writes(m_exec.getValue());
        if (m_dtor.getValue().size()) {
            out->writes("\n");
        }
    }
    out->writes(m_dtor.getValue());

    if (m_new_scope) {
        out->dec_ind();
        out->println("end");
    }
}

}
}
}
