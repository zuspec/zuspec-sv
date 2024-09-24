/**
 * TaskIsValueAggregate.h
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

namespace zsp {
namespace sv {
namespace gen {
namespace exec {

class TaskGenerate;

class TaskIsValueAggregate :
    public virtual arl::dm::VisitorBase {
public:

    virtual ~TaskIsValueAggregate() { }

    virtual bool check(vsc::dm::IAccept *t) {
        m_ret = false;
        t->accept(m_this);
        return m_ret;
    }

    virtual void visitDataTypeAction(arl::dm::IDataTypeAction *t) override { }

    virtual void visitDataTypeComponent(arl::dm::IDataTypeComponent *t) override { }

    virtual void visitDataTypeStruct(vsc::dm::IDataTypeStruct *t) override { 
        m_ret = true;
    }
    
    virtual void visitDataTypeArray(vsc::dm::IDataTypeArray *t) override {
        m_ret = true;
    }

protected:
    bool                    m_ret;
};

} /* namespace exec */
} /* namespace gen */
} /* namespace sv */
} /* namespace zsp */


