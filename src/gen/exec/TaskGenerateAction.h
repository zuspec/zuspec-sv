/**
 * TaskGenerateAction.h
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



class TaskGenerateAction :
    public virtual TaskGenerateStruct {
public:
    TaskGenerateAction(
        TaskGenerate        *gen,
        IOutput             *out);

    virtual ~TaskGenerateAction();

    virtual void generate(vsc::dm::IDataTypeStruct *t) override;

    virtual void generate_head(vsc::dm::IDataTypeStruct *t) override;

    virtual void generate_fields(vsc::dm::IDataTypeStruct *t) override;

    virtual void generate_constraints(vsc::dm::IDataTypeStruct *t) override;

    virtual void generate_ctor(vsc::dm::IDataTypeStruct *t) override;

    virtual void generate_dtor(vsc::dm::IDataTypeStruct *t) override { }

    virtual void generate_init(vsc::dm::IDataTypeStruct *t) override { }

    virtual void generate_create_assign(vsc::dm::IDataTypeStruct *t) override { }

    virtual void generate_create_default(vsc::dm::IDataTypeStruct *t) override { }

    virtual void generate_create_init(vsc::dm::IDataTypeStruct *t) override { }

    virtual void generate_execs(vsc::dm::IDataTypeStruct *t) override;

    virtual void generate_activity(vsc::dm::IDataTypeStruct *t);

    virtual void generate_methods(vsc::dm::IDataTypeStruct *t) override;

    virtual void visitTypeFieldRef(vsc::dm::ITypeFieldRef *f) override;

};

}
}
}
}


