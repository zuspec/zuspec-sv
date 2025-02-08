/*
 * TaskGenerateImportApi.cpp
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
#include "GenRefExprExecModel.h"
#include "TaskGenerate.h"
#include "TaskGenerateDataType.h"
#include "TaskGenerateImportApi.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateImportApi::TaskGenerateImportApi(
        TaskGenerate        *gen,
        IOutput             *out) : m_gen(gen), m_out(out) {
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateImportApi", gen->getDebugMgr());
}

TaskGenerateImportApi::~TaskGenerateImportApi() {

}

void TaskGenerateImportApi::generate(const std::vector<arl::dm::IDataTypeFunction *> &funcs) {
    m_out->println("class pss_import_api #(type BaseT=zsp_sv::empty_t) extends backend_api #(BaseT);");
    m_out->inc_ind();
    for (std::vector<arl::dm::IDataTypeFunction *>::const_iterator
        it=funcs.begin();
        it!=funcs.end(); it++) {
        (*it)->accept(m_this);
    }
    m_out->dec_ind();
    m_out->println("endclass");
}

void TaskGenerateImportApi::visitDataTypeFunction(arl::dm::IDataTypeFunction *f) {
    DEBUG_ENTER("visitDataTypeFunction %s", f->name().c_str());
    if (f->getImportSpecs().size() > 0) {
        std::string name;

        GenRefExprExecModel genref(m_gen, 0, "", false);
        bool is_ctxt = f->hasFlags(arl::dm::DataTypeFunctionFlags::Context);
    
        if (is_ctxt) {
            int idx = f->name().rfind("::");
            if (idx != -1) {
                name = f->name().substr(idx+2);
            } else {
                name = f->name();
            }
        } else {
            name = m_gen->getNameMap()->getName(f);
        }
    
        bool is_task = f->hasFlags(arl::dm::DataTypeFunctionFlags::Target);
    
        m_out->indent();
        if (is_task) {
            m_out->write("virtual task %s(\n", name.c_str());
            m_out->inc_ind();
            m_out->inc_ind();
            if (f->getReturnType()) {
                m_out->indent();
                m_out->write("output ");
                TaskGenerateDataType(m_gen, m_out).generate(f->getReturnType());
                m_out->write(" __retval,\n");
            }
        } else {
            m_out->write("virtual function ");
            if (f->getReturnType()) {
                TaskGenerateDataType(m_gen, m_out).generate(f->getReturnType());
            } else {
                m_out->write("void");
            }
            m_out->write(" %s(\n", name.c_str());
            m_out->inc_ind();
            m_out->inc_ind();
        }
        // We're indented for the remaining parameters
        for (std::vector<arl::dm::IDataTypeFunctionParamDecl *>::const_iterator
            it=f->getParameters().begin();
            it!=f->getParameters().end(); it++) {
            m_out->indent();
            switch ((*it)->getDirection()) {
                case arl::dm::ParamDir::In: m_out->write("input "); break;
                case arl::dm::ParamDir::Out: m_out->write("output "); break;
                case arl::dm::ParamDir::InOut: m_out->write("inout "); break;
            }
            TaskGenerateDataType(m_gen, m_out).generate((*it)->getDataType());
            m_out->write(" %s%s\n",
                (*it)->name().c_str(),
                (it+1 != f->getParameters().end())?",":");");
        }
        m_out->dec_ind();

        m_out->println("`ZSP_FATAL((\"Import function %s is not implemented\"));", name.c_str());
        // 
        m_out->dec_ind();
        m_out->println("end%s", is_task?"task":"function");
    }
    DEBUG_LEAVE("visitDataTypeFunction");
}

dmgr::IDebug *TaskGenerateImportApi::m_dbg = 0;

}
}
}
}
