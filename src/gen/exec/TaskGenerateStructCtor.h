/**
 * TaskGenerateStructCtor.h
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
#include "dmgr/IDebugMgr.h"
#include "zsp/arl/dm/impl/VisitorBase.h"
#include "gen/IOutput.h"

namespace zsp {
namespace sv {
namespace gen {
namespace exec {

class TaskGenerate;

class TaskGenerateStructCtor :
    virtual public arl::dm::VisitorBase {
public:
    TaskGenerateStructCtor(
        TaskGenerate        *gen,
        IOutput             *out);

    virtual ~TaskGenerateStructCtor();

    virtual void generate_head(vsc::dm::IDataTypeStruct *t);

    virtual void generate(vsc::dm::IDataTypeStruct *t);

    virtual void generate_tail(vsc::dm::IDataTypeStruct *t);

    virtual void visitTypeField(vsc::dm::ITypeField *f) override;

protected:
    dmgr::IDebug                *m_dbg;
    TaskGenerate                *m_gen;
    IOutput                     *m_out;
    vsc::dm::ITypeField         *m_field;

};

}
}
}
}


