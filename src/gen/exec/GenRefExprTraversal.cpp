/*
 * GenRefExprTraversal.cpp
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
#include "dmgr/impl/DebugMacros.h"
#include "GenRefExprTraversal.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


GenRefExprTraversal::GenRefExprTraversal(
        TaskGenerate                *gen,
        vsc::dm::IDataTypeStruct    *ctxt,
        const std::string           &ctxtRef,
        bool                        ctxtPtr,
        vsc::dm::IDataTypeStruct    *parent) :
    GenRefExprExecModel(gen, parent, "__parent", false, "", false),
    m_actionRef(ctxtRef), m_action(ctxt) {

}

GenRefExprTraversal::~GenRefExprTraversal() {

}

void GenRefExprTraversal::visitTypeExprRefInline(vsc::dm::ITypeExprRefInline *e) {
    DEBUG_ENTER("visitTypeExprRefInline");
    std::string ret = m_actionRef + ".";
    m_out_l.push_back(ret);
    m_type_l.push_back((m_inline_s.size())?m_inline_s.back():m_action);
    DEBUG_LEAVE("visitTypeExprRefInline %p", m_type_l.back());
}

}
}
}
}
