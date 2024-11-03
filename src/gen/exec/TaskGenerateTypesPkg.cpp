/*
 * TaskGenerateTypesPkg.cpp
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
#include "gen/Output.h"
#include "gen/TaskBuildTypeCollection.h"
#include "ICustomGen.h"
#include "TaskGenerateImportApi.h"
#include "TaskGenerateTypesPkg.h"
#include "TaskDefineType.h"
#include "gen/Output.h"


namespace zsp {
namespace sv {
namespace gen {
namespace exec {


TaskGenerateTypesPkg::TaskGenerateTypesPkg(
        dmgr::IDebugMgr         *dmgr,
        arl::dm::IContext       *ctxt,
        arl::eval::IFactory     *eval_f,
        std::ostream            *out) : TaskGenerate(dmgr, ctxt, eval_f, out) {
    m_dbg = 0;
    DEBUG_INIT("zsp::sv::gen::exec::TaskGenerateActorPkg", dmgr);
}

TaskGenerateTypesPkg::~TaskGenerateTypesPkg() {

}

bool TaskGenerateTypesPkg::generate() {
    OutputUP out(new Output(m_out, false));

    attach_custom_gen();

    out->println("`include \"zsp_sv_macros.svh\"");
    out->println("package pss_types;");
    out->inc_ind();
    out->println("import zsp_sv::*;");
    out->println("");
    out->println("typedef class pss_import_api;");
    out->println("// TODO: define model-specific executor class");
    out->println("typedef executor_base executor_base_c;");
    out->println("typedef executor_base executor_t;");
    TypeCollectionUP types(TaskBuildTypeCollection(m_dmgr).build(m_ctxt));
    std::vector<int32_t> sorted = types->sort();

    std::set<std::string> omitted = {
        "addr_handle_t"
    };
    for (std::vector<int32_t>::const_iterator
        it=sorted.begin();
        it!=sorted.end(); it++) {
        vsc::dm::IDataType *t = types->getType(*it);
        if (omitted.find(getNameMap()->getName(t)) == omitted.end()) {
            out->println("typedef class %s;", getNameMap()->getName(t).c_str());
        }
    }
    out->println("");

    for (std::vector<int32_t>::const_iterator
        it=sorted.begin();
        it!=sorted.end(); it++) {
        if (it != sorted.begin()) {
            out->println("");
        }
        ICustomGen *custom_gen = dynamic_cast<ICustomGen *>(
            types->getType(*it)->getAssociatedData());
        if (custom_gen) {
            custom_gen->genDefinition(
                this, 
                out.get(),
                types->getType(*it));
        } else {
            TaskDefineType(this, out.get()).generate(
                types->getType(*it));
        }
    }
    out->println("");

    for (std::vector<arl::dm::IDataTypeFunction *>::const_iterator
        it=m_ctxt->getDataTypeFunctions().begin();
        it!=m_ctxt->getDataTypeFunctions().end(); it++) {
        // TODO: filter out specific functions
        if (!(*it)->hasFlags(arl::dm::DataTypeFunctionFlags::Context) 
            && !(*it)->hasFlags(arl::dm::DataTypeFunctionFlags::Core)) {
            TaskDefineType(this, out.get()).generate(*it);
        }
    }

    out->println("");
    TaskGenerateImportApi(this, out.get()).generate(
        m_ctxt->getDataTypeFunctions());
    out->println("");
    out->dec_ind();
    out->println("endpackage");

    out->close();
    return true;
}

}
}
}
}
