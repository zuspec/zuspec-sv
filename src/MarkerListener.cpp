/*
 * MarkerListener.cpp
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
#include <string.h>
#include "MarkerListener.h"
#include "ZuspecSvDpiImp.h"


namespace zsp {
namespace sv {


MarkerListener::MarkerListener() {
    memset(m_hasSeverity, 0, sizeof(m_hasSeverity));
}

MarkerListener::~MarkerListener() {

}

void MarkerListener::marker(const zsp::parser::IMarker *m) {
    m_hasSeverity[(int)m->severity()] = true;
    zuspec_error(m->msg().c_str());
}

bool MarkerListener::hasSeverity(zsp::parser::MarkerSeverityE s) {
    return m_hasSeverity[(int)s];
}

}
}
