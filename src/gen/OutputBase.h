/**
 * OutputBase.h
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
#include <string>
#include <stdarg.h>
#include "IOutput.h"

namespace zsp {
namespace sv {
namespace gen {



class OutputBase : public virtual IOutput {
public:
    OutputBase(const std::string &ind="");

    virtual ~OutputBase();

    /**
     * @brief Writes indent, content, then a newline
     * 
     * @param fmt 
     * @param ... 
     */
    virtual void println(const char *fmt, ...) override;

    /**
     * @brief Writes indent and content without a newline
     * 
     * @param fmt 
     * @param ... 
     */
    virtual void print(const char *fmt, ...) override;

    /**
     * @brief Writes content only
     * 
     * @param fmt 
     * @param ... 
     */
    virtual void write(const char *fmt, ...) override;

    virtual void close() override;

    /**
     * @brief Writes the current indent
     * 
     */
    virtual void indent() override;

    virtual void inc_ind() override;

    virtual void dec_ind() override;

    virtual const char *ind() const override {
        return m_ind.c_str();
    }

protected:
    std::string                 m_ind;

};

}
}
}


