/*
 * ZuspecSv.cpp
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
#include <stdio.h>
#include <fstream>
#include <sstream>
#include "dmgr/FactoryExt.h"
#include "vsc/dm/FactoryExt.h"
#include "vsc/solvers/FactoryExt.h"
#include "zsp/arl/dm/FactoryExt.h"
#include "zsp/parser/FactoryExt.h"
#include "zsp/fe/parser/FactoryExt.h"
#include "zsp/ast/IFactory.h"
#include "Actor.h"
#include "EvalBackendProxy.h"
#include "MarkerListener.h"
#include "ZuspecSv.h"
#include "ZuspecSvDpiImp.h"

extern "C" zsp::ast::IFactory *ast_getFactory();

namespace zsp {
namespace sv {



ZuspecSv::ZuspecSv() : 
    m_initialized(false),
    m_loaded(false) {
    m_solver_f = vsc_solvers_getFactory();


}

ZuspecSv::~ZuspecSv() {

}

ZuspecSv *ZuspecSv::inst() {
    if (!m_inst) {
        m_inst = ZuspecSvUP(new ZuspecSv());
    }
    return m_inst.get();
}

bool ZuspecSv::init(
    const std::string       &pss_files,
    bool                    load) {
    if (m_initialized) {
        return true;
    }

    m_dmgr = dmgr_getFactory()->getDebugMgr();
    m_pssfiles = pss_files;

    vsc::dm::IFactory *vsc_dm_f = vsc_dm_getFactory();
    vsc_dm_f->init(m_dmgr);

    arl::dm::IFactory *arl_dm_f = zsp_arl_dm_getFactory();
    arl_dm_f->init(m_dmgr);



    m_ctxt = arl::dm::IContextUP(arl_dm_f->mkContext(
        vsc_dm_f->mkContext()));

    if (load) {
        if (!ensureLoaded()) {
            return false;
        }
    }

    // Process plusargs
    /*
    s_vpi_vlog_info_t info;
    vpi_get_vlog_info(&info);

    for (int32_t i=0; i<info.argc; i++) {
        fprintf(stdout, "Arg: %s\n", info.argv[i]);
    }
     */

    // TODO: Load source files if plusarg speciifed

//    parser::IAstBuilderUP builder(parser_f->mkAstBuilder());

    m_initialized = true;

    return true;
}

bool ZuspecSv::ensureLoaded() {
    char tmp[1024];
    if (m_loaded) {
        return true;
    }

    if (m_pssfiles == "") {
        zuspec_message("No PSS files specified");
        return false;
    }

    snprintf(tmp, sizeof(tmp), "Parsing %s", m_pssfiles.c_str());
    zuspec_message(tmp);

    MarkerListener listener;
    parser::IFactory *parser_f = zsp_parser_getFactory();
    parser_f->init(
        m_dmgr,
        ast_getFactory());
    parser::IAstBuilderUP builder(parser_f->mkAstBuilder(&listener));
    ast::IGlobalScopeUP global(parser_f->getAstFactory()->mkGlobalScope(0));


    std::fstream s;

    s.open(m_pssfiles, std::fstream::in);

    if (!s.is_open()) {
        snprintf(tmp, sizeof(tmp), "Failed to open file %s", m_pssfiles.c_str());
        zuspec_fatal(tmp);
        return false;
    }

    zuspec_message("--> Calling build");
    builder->build(
        global.get(),
        &s);
    zuspec_message("<-- Calling build");
    
    if (listener.hasSeverity(parser::MarkerSeverityE::Error)) {
        zuspec_fatal("Parse errors");
        return false;
    }

    parser::ILinkerUP linker(parser_f->mkAstLinker());
    ast::ISymbolScopeUP scope(linker->link(
        &listener,
        {global.get()}
    ));

    if (listener.hasSeverity(parser::MarkerSeverityE::Error)) {
        zuspec_fatal("Linking errors");
        return false;
    }

    fe::parser::IFactory *fe_parser_f = zsp_fe_parser_getFactory();
    fe_parser_f->init(m_dmgr, parser_f);
    fe::parser::IAst2ArlContextUP builder_ctxt(fe_parser_f->mkAst2ArlContext(
        m_ctxt.get(),
        scope.get(),
        &listener
    ));
    fe::parser::IAst2ArlBuilderUP fe_builder(fe_parser_f->mkAst2ArlBuilder());
    fe_builder->build(
        scope.get(),
        builder_ctxt.get()
    );

    if (listener.hasSeverity(parser::MarkerSeverityE::Error)) {
        zuspec_fatal("Data-model build errors");
        return false;
    }


    m_loaded = true;

    return true;
}

ZuspecSvUP ZuspecSv::m_inst;

}
}

/****************************************************************************
 * DPI Interface
 ****************************************************************************/

extern "C" uint32_t zuspec_init(
    const char      *pss_files,
    int             load) {
    zuspec_message("Hello World!");
    return zsp::sv::ZuspecSv::inst()->init(pss_files, load);
}

extern "C" chandle zuspec_Actor_new(
    const char          *randstate,
    const char          *comp_t_s,
    const char          *action_t_s,
    uint64_t             backend_h) {
    char tmp[1024];
    zsp::sv::ZuspecSv *zsp_sv = zsp::sv::ZuspecSv::inst();
    zsp::arl::dm::IContext *ctxt = zsp_sv->ctxt();
    zsp::sv::EvalBackendProxy *backend = reinterpret_cast<zsp::sv::EvalBackendProxy *>(backend_h);

    if (!zsp_sv->ensureLoaded()) {
        zuspec_fatal("Failed to load PSS files");
        return 0;
    }

    zsp::arl::dm::IDataTypeComponent *comp_t = ctxt->findDataTypeComponent(comp_t_s);
    if (!comp_t) {
        snprintf(tmp, sizeof(tmp), "Failed to find component %s", comp_t_s);
        zuspec_fatal(tmp);
        return 0;
    }

    zsp::arl::dm::IDataTypeAction *action_t = ctxt->findDataTypeAction(action_t_s);

    if (!action_t) {
        snprintf(tmp, sizeof(tmp), "Failed to find action %s", action_t_s);
        zuspec_fatal(tmp);
        return 0;
    }

    zsp::sv::Actor *actor = new zsp::sv::Actor(
        comp_t,
        action_t,
        backend);

    backend->emitMessage("Hello World");

    return reinterpret_cast<chandle>(actor);
}
