/**
 * TaskGenerateAddrRegion.h
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
#include "TaskGenerateStruct.h"

namespace zsp {
namespace sv {
namespace gen {
namespace exec {



class TaskGenerateAddrRegion :
    public virtual TaskGenerateStruct {
public:
    TaskGenerateAddrRegion(
        TaskGenerate        *gen,
        IOutput             *out
    );

    virtual ~TaskGenerateAddrRegion();

    virtual void generate_head(vsc::dm::IDataTypeStruct *t) override;

    virtual void generate_fields(vsc::dm::IDataTypeStruct *t) override;

};

}
}
}
}

