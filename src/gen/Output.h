/**
 * Output.h
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
#include <iostream>
#include <string>
#include "IOutput.h"
#include "OutputBase.h"

namespace zsp {
namespace sv {
namespace gen {


class Output : 
    public virtual IOutput,
    public virtual OutputBase {
public:
    Output(
        std::ostream            *out,
        bool                    owned,
        const std::string       &ind=""
    );

    virtual ~Output();

    virtual void close() override;

    virtual void writes(const std::string &str) override;

private:
    std::ostream                *m_out;
    bool                        m_owned;

};

}
}
}


