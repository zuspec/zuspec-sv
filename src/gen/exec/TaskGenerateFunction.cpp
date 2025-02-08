/*
 * TaskGenerateFunction.cpp
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
#include "TaskGenerate.h"
#include "TaskGenerateDataType.h"
#include "TaskGenerateExecScope.h"
#include "TaskGenerateFunction.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateFunction::TaskGenerateFunction(
    TaskGenerate        *gen,
    IGenRefExpr         *genref,
    IOutput             *out) : m_gen(gen), m_genref(genref), m_out(out) {

}

TaskGenerateFunction::~TaskGenerateFunction() {

}

void TaskGenerateFunction::generate(
    arl::dm::IDataTypeFunction          *f,
    bool                                is_ctxt) {
    std::string name;

    is_ctxt = f->hasFlags(arl::dm::DataTypeFunctionFlags::Context);

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

    bool is_task = (
        f->hasFlags(arl::dm::DataTypeFunctionFlags::Target)
        || f->hasFlags(arl::dm::DataTypeFunctionFlags::Blocks));

    m_out->indent();
    if (is_task) {
        m_out->write("%stask%s%s(\n", 
            (is_ctxt)?"virtual ":"",
            (is_ctxt)?" ":" automatic ",
            name.c_str());
        m_out->inc_ind();
        m_out->inc_ind();
        if (f->getReturnType()) {
            m_out->indent();
            m_out->write("output ");
            TaskGenerateDataType(m_gen, m_out).generate(f->getReturnType());
            m_out->write(" __retval,\n");
        }
    } else {
        m_out->write("%sfunction%s", 
            (is_ctxt)?"virtual ":"",
            (!is_ctxt)?" automatic ":" ");
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
    m_out->println("input executor_base exec_b%s", (f->getParameters().size())?",":"");
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
    m_genref->pushScope(f->getParamScope());
    TaskGenerateExecScope(m_gen, m_genref, m_out).generate(f->getBody(), false);
    m_genref->popScope();

    // 
    m_out->dec_ind();
    m_out->println("end%s", is_task?"task":"function");
}

}
}
}
}
