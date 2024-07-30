/**
 * OutputStr.h
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
#pragma once
#include <sstream>
#include "IOutputStr.h"
#include "OutputBase.h"

namespace zsp {
namespace sv {
namespace gen {


class OutputStr : 
    public virtual IOutputStr, 
    public virtual OutputBase {
public:
    OutputStr(
        const std::string       &ind=""
    );

    OutputStr(const OutputStr &o) {
        const std::string &ov = o.m_sstr.str();
        m_sstr.write(ov.c_str(), ov.size());
        m_ind = o.m_ind;
    }

    virtual ~OutputStr();

    virtual std::string getValue() const override {
        return m_sstr.str();
    }

    virtual void writes(const std::string &str) override;

    void clear();

private:
    std::stringstream           m_sstr;

};

}
}
}


