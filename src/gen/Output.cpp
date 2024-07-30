/*
 * Output.cpp
 *
 * Copyright 2022 Matthew Ballance and Contributors
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
#include <fstream>
#include <stdarg.h>
#include <stdio.h>
#include "Output.h"


namespace zsp {
namespace sv {
namespace gen {


Output::Output(
    std::ostream            *out,
    bool                    owned,
    const std::string       &ind) : OutputBase(ind), m_out(out), m_owned(owned) {

}

Output::~Output() {
    if (m_owned && m_out) {
        delete m_out;
    }
}

void Output::writes(const std::string &str) {
    m_out->write(str.c_str(), str.size());
}

void Output::close() {
    m_out->flush();
    if (dynamic_cast<std::fstream *>(m_out)) {
        dynamic_cast<std::fstream *>(m_out)->close();
    }
    m_out = 0;
}

}
}
}
