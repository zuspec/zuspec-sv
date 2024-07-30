/*
 * OutputBase.cpp
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
#include "OutputBase.h"


namespace zsp {
namespace sv {
namespace gen {


OutputBase::OutputBase(const std::string &ind) : m_ind(ind) {

}

OutputBase::~OutputBase() {

}

/**
 * @brief Writes indent, content, then a newline
 * 
 * @param fmt 
 * @param ... 
 */
void OutputBase::println(const char *fmt, ...) {
    char tmp[1024];
    va_list ap;
    va_start(ap, fmt);

    int len = vsnprintf(tmp, sizeof(tmp), fmt, ap);
    if (m_ind.size() > 0) {
        writes(m_ind);
    }
    writes(tmp);
    writes("\n");

    va_end(ap);
}

/**
 * @brief Writes indent and content without a newline
 * 
 * @param fmt 
 * @param ... 
 */
void OutputBase::print(const char *fmt, ...) {
    char tmp[1024];
    va_list ap;
    va_start(ap, fmt);

    int len = vsnprintf(tmp, sizeof(tmp), fmt, ap);
    if (m_ind.size() > 0) {
        writes(m_ind);
    }
    writes(tmp);

    va_end(ap);
}

/**
 * @brief Writes content only
 * 
 * @param fmt 
 * @param ... 
 */
void OutputBase::write(const char *fmt, ...) {
    char tmp[1024];
    va_list ap;
    va_start(ap, fmt);

    int len = vsnprintf(tmp, sizeof(tmp), fmt, ap);
    writes(tmp);

    va_end(ap);
}

void OutputBase::close() {

}

/**
 * @brief Writes the current indent
 * 
 */
void OutputBase::indent() {
    if (m_ind.size() > 0) {
        writes(m_ind);
    }
}

void OutputBase::inc_ind() {
    m_ind += "    ";
}

void OutputBase::dec_ind() {
    if (m_ind.size() > 4) {
        m_ind = m_ind.substr(4);
    } else {
        m_ind = "";
    }
}

}
}
}
