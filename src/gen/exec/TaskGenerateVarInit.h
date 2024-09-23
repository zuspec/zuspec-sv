/**
 * TaskGenerateVarInit.h
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
#include "IGenRefExpr.h"
#include "gen/IOutput.h"

namespace zsp {
namespace sv {
namespace gen {
namespace exec {

class TaskGenerate;

class TaskGenerateVarInit :
    public virtual arl::dm::VisitorBase {
public:
    TaskGenerateVarInit(
        TaskGenerate            *gen,
        IGenRefExpr             *genref,
        IOutput                 *out
    );

    virtual ~TaskGenerateVarInit();

    virtual void generate(arl::dm::ITypeProcStmtVarDecl *var);

    virtual void visitDataTypeBool(vsc::dm::IDataTypeBool *t) override;

    virtual void visitDataTypeInt(vsc::dm::IDataTypeInt *t) override;

    virtual void visitDataTypeString(vsc::dm::IDataTypeString *t) override;

    virtual void visitDataTypeStruct(vsc::dm::IDataTypeStruct *t) override;

protected:
    static dmgr::IDebug             *m_dbg;
    TaskGenerate                    *m_gen;
    IGenRefExpr                     *m_genref;
    IOutput                         *m_out;
    arl::dm::ITypeProcStmtVarDecl   *m_var;
};

}
}
}
}


