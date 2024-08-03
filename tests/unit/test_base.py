#****************************************************************************
#* test_base.py
#*
#* Copyright 2022 Matthew Ballance and Contributors
#*
#* Licensed under the Apache License, Version 2.0 (the "License"); you may 
#* not use this file except in compliance with the License.  
#* You may obtain a copy of the License at:
#*
#*   http://www.apache.org/licenses/LICENSE-2.0
#*
#* Unless required by applicable law or agreed to in writing, software 
#* distributed under the License is distributed on an "AS IS" BASIS, 
#* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
#* See the License for the specific language governing permissions and 
#* limitations under the License.
#*
#* Created on:
#*     Author: 
#*
#****************************************************************************
import io
import os
import sys
from unittest import TestCase

sys.path.insert(0,
    os.path.join(
        os.path.dirname(os.path.dirname(
            os.path.dirname(os.path.abspath(__file__)))),
        "python"))

import zsp_sv.core as zsp_sv

class TestBase(TestCase):

    def setUp(self) -> None:
        self.zsp_sv_f = zsp_sv.Factory.inst()
        return super().setUp()

    def tearDown(self) -> None:
        return super().tearDown()
    
    def buildModelGetRoots(
            self,
            content,
            comp_t,
            action_t,
            load_stdlib=True):
        import zsp_fe_parser.core as zsp_fe
        import zsp_arl_dm.core as zsp_arl
        import zsp_parser.core as zspp

        factory = zsp_fe.Factory.inst()
        factory.getDebugMgr().enable(True)

        arl_f = zsp_arl.Factory.inst()
        arl_ctxt = arl_f.mkContext()

        zsp_f = zspp.Factory.inst()
        marker_c = zsp_f.mkMarkerCollector()
        ast_builder = zsp_f.mkAstBuilder(marker_c)
        ast_linker = zsp_f.mkAstLinker()
        zsp_fe_f = zsp_fe.Factory.inst()

        ast_l = []
        if load_stdlib:
            core_lib = zsp_f.getAstFactory().mkGlobalScope(0)
            zsp_f.loadStandardLibrary(ast_builder, core_lib)
            ast_l.append(core_lib)

        ast_root = zsp_f.getAstFactory().mkGlobalScope(1)
        ast_builder.build(ast_root, io.StringIO(content))
        ast_l.append(ast_root)

        for m in marker_c.markers():
            print("Parse Marker: %s" % m.msg())
        self.assertFalse(marker_c.hasSeverity(zspp.MarkerSeverityE.Error))

        
        linked_root = ast_linker.link(marker_c, ast_l)
        for m in marker_c.markers():
            print("Linker Marker: %s" % m.msg())
        self.assertFalse(marker_c.hasSeverity(zspp.MarkerSeverityE.Error))

        ast2arl_builder = zsp_fe_f.mkAst2ArlBuilder()
        ast2arl_ctxt = zsp_fe_f.mkAst2ArlContext(
            arl_ctxt,
            linked_root,
            marker_c)
        
        ast2arl_builder.build(linked_root, ast2arl_ctxt)

        pss_top = arl_ctxt.findDataTypeStruct(comp_t)
        self.assertIsNotNone(pss_top)
        print("pss_top=%s" % str(pss_top))

        pss_top_Entry = arl_ctxt.findDataTypeStruct(action_t)
        self.assertIsNotNone(pss_top_Entry)

        return (arl_ctxt, pss_top, pss_top_Entry)


