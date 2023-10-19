/**
 * ZuspecSv.h
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
#include <memory>
#include <stdint.h>
#include <string>
#include "dmgr/IDebugMgr.h"
#include "vsc/solvers/IFactory.h"
#include "vsc/solvers/IRandState.h"
#include "zsp/arl/dm/IContext.h"

namespace zsp {
namespace sv {


class ZuspecSv;
using ZuspecSvUP=std::unique_ptr<ZuspecSv>;
class ZuspecSv {
public:
    ZuspecSv();

    virtual ~ZuspecSv();

    bool init(
        const std::string   &pss_files,
        bool                load,
        bool                debug);

    bool ensureLoaded();

    arl::dm::IContext *ctxt() const {
        return m_ctxt.get();
    }

    static ZuspecSv *inst();

private:

private:
    static ZuspecSvUP           m_inst;
    dmgr::IDebugMgr             *m_dmgr;
    std::string                 m_pssfiles;
    bool                        m_initialized;
    bool                        m_loaded;
    vsc::solvers::IFactory      *m_solver_f;
    vsc::solvers::IRandStateUP  m_randstate_glbl;
    arl::dm::IContextUP         m_ctxt;

};

}
}


