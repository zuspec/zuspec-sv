/**
 * TaskGenerateStructDoSolveExec.h
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
#include <vector>
#include "dmgr/IDebugMgr.h"
#include "zsp/arl/dm/impl/VisitorBase.h"
#include "zsp/arl/dm/ITypeExecProc.h"
#include "gen/IOutput.h"
#include "IGenRefExpr.h"

namespace zsp {
namespace sv {
namespace gen {
namespace exec {

class TaskGenerate;

class TaskGenerateStructDoSolveExec :
    public virtual arl::dm::VisitorBase {
public:
    TaskGenerateStructDoSolveExec(
        TaskGenerate        *gen,
        IOutput             *out);

    virtual ~TaskGenerateStructDoSolveExec();

    void generate(
        vsc::dm::IDataTypeStruct *t,
        std::string &kind);

    virtual void visitDataTypeStruct(vsc::dm::IDataTypeStruct *t) override;

    virtual void visitTypeFieldPhy(vsc::dm::ITypeFieldPhy *f) override;

    virtual void visitTypeFieldRef(vsc::dm::ITypeFieldRef *f) override;

protected:
    static dmgr::IDebug             *m_dbg;
    TaskGenerate                    *m_gen;
    IOutput                         *m_out;
    std::string                     m_kind;
    vsc::dm::ITypeField             *m_field;

};

}
}
}
}

