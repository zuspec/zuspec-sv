#****************************************************************************
#* cmd_gen_sv_actor.py
#*
#* Copyright 2023 Matthew Ballance and Contributors
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

class CmdGenSvActor(object):

    def __call__(self, args):
        import zsp_fe_parser.core as zsp_fe
        import zsp_arl_dm.core as zsp_arl
        import zsp_parser.core as zspp
        import zsp_sv.core as zsp_sv

        if args.action is None:
            raise Exception("Root action (-action ) not specified")

        files = args.files if isinstance(args.files, list) else [args.files]

        if len(files) == 0:
            raise Exception("No files specified")

        factory = zsp_fe.Factory.inst()
        if args.debug:
            factory.getDebugMgr().enable(True)

        arl_f = zsp_arl.Factory.inst()
        arl_ctxt = arl_f.mkContext()

        zsp_f = zspp.Factory.inst()
        marker_c = zsp_f.mkMarkerCollector()
        ast_builder = zsp_f.mkAstBuilder(marker_c)
        ast_linker = zsp_f.mkAstLinker()
        zsp_fe_f = zsp_fe.Factory.inst()

        load_stdlib = True

        ast_l = []
        if load_stdlib:
            core_lib = zsp_f.getAstFactory().mkGlobalScope(0)
            zsp_f.loadStandardLibrary(ast_builder, core_lib)
            ast_l.append(core_lib)

        for file in files:
            ast_root = zsp_f.getAstFactory().mkGlobalScope(1)
            with open(file, "r") as fp:
                ast_builder.build(ast_root, fp)
            ast_l.append(ast_root)

        for m in marker_c.markers():
            print("Parse Marker: %s" % m.msg())
        if marker_c.hasSeverity(zspp.MarkerSeverityE.Error):
            raise Exception("Failed to parse")

        
        linked_root = ast_linker.link(marker_c, ast_l)
        for m in marker_c.markers():
            print("Linker Marker: %s" % m.msg())
        if marker_c.hasSeverity(zspp.MarkerSeverityE.Error):
            raise Exception("Failed to link")

        ast2arl_builder = zsp_fe_f.mkAst2ArlBuilder()
        ast2arl_ctxt = zsp_fe_f.mkAst2ArlContext(
            arl_ctxt,
            linked_root,
            marker_c)
        
        ast2arl_builder.build(linked_root, ast2arl_ctxt)

        pss_top = arl_ctxt.findDataTypeStruct(args.comp)
        if pss_top is None:
            raise Exception("pss_top %s could not be located" % args.comp)

        pss_top_Entry = arl_ctxt.findDataTypeStruct(args.action)
        if pss_top_Entry is None:
            raise Exception("root action %s could not be found" % args.action)
        
        zsp_sv_f = zsp_sv.Factory.inst()

        zsp_sv_f.prepContextExec(arl_ctxt)

        with open(args.output, "w") as fp:
            generator = zsp_sv_f.mkGenerateTypesPkg(
                arl_ctxt,
                fp)
            generator.generate()
            generator = zsp_sv_f.mkGenerateActorPkgPrv(
                arl_ctxt,
                pss_top,
                pss_top_Entry,
                fp)
            generator.generate()
            generator = zsp_sv_f.mkGenerateActorPkg(
                arl_ctxt,
                pss_top,
                pss_top_Entry,
                fp)
            generator.generate()
        pass

    @staticmethod
    def register(cmd, subparsers):
        parser = subparsers.add_parser("synth.sv.actor",
                        help="Create a SystemVerilog implementation of a PSS model")
        parser.set_defaults(func=CmdGenSvActor())
        parser.add_argument("-o", "--output", help="Output path", default="pss_top_sv.sv")
        parser.add_argument("-d", "--debug", 
                            help="Enable debug output",
                            action="store_true")
        parser.add_argument("-comp", help="Specifies root component", default="pss_top")
        parser.add_argument("-action", help="Specifies root action")
        parser.add_argument("files", action="append")
