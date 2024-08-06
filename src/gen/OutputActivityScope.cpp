/*
 * OutputActivityScope.cpp
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
#include "OutputActivityScope.h"


namespace zsp {
namespace sv {
namespace gen {


OutputActivityScope::OutputActivityScope(IOutput *upper) :
        m_decl(upper->ind()), m_run(upper->ind()) {
}

OutputActivityScope::~OutputActivityScope() {

}

void OutputActivityScope::apply(IOutput *out) {
    out->writes(m_decl.getValue());
    if (m_decl.getValue().size()) {
        out->writes("\n");
    }
    out->writes(m_run.getValue());
    if (m_run.getValue().size()) {
        out->writes("\n");
    }
}

}
}
}
