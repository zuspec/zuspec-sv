/**
 * TaskGenerateDataType.h
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

class TaskGenerator;

class TaskGenerateDataType :
    public virtual arl::dm::VisitorBase {
public:
    TaskGenerateDataType(
        TaskGenerate        *gen,
        IOutput             *out
    );

    virtual ~TaskGenerateDataType();

    virtual void generate(vsc::dm::IDataType *t);

	virtual void visitDataTypeBool(vsc::dm::IDataTypeBool *t) override;

	virtual void visitDataTypeEnum(vsc::dm::IDataTypeEnum *t) override;

	virtual void visitDataTypeInt(vsc::dm::IDataTypeInt *t) override;

	virtual void visitDataTypePtr(vsc::dm::IDataTypePtr *t) override;

	virtual void visitDataTypeString(vsc::dm::IDataTypeString *t) override;

    virtual void visitDataTypeStruct(vsc::dm::IDataTypeStruct *t) override;

protected:
    dmgr::IDebug                *m_dbg;
    TaskGenerate                *m_gen;
    IOutput                     *m_out;
};

}
}
}
}


