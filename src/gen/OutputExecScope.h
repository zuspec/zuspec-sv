/**
 * OutputExecScope.h
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
#include "IOutput.h"
#include "OutputBase.h"
#include "OutputStr.h"

namespace zsp {
namespace sv {
namespace gen {



class OutputExecScope {
public:
    OutputExecScope(
        bool                new_scope,
        const std::string   &ind="");

    OutputExecScope(
        bool                new_scope,
        IOutput             *upper);

    virtual ~OutputExecScope();

    void apply(IOutput *out);

    IOutput *decl() { return &m_decl; }

    IOutput *init() { return &m_init; }

    IOutput *exec() { return &m_exec; }

    IOutput *dtor() { return &m_dtor; }

private:
    bool            m_new_scope;
    OutputStr       m_decl;
    OutputStr       m_init;
    OutputStr       m_exec;
    OutputStr       m_dtor;

};

}
}
}


