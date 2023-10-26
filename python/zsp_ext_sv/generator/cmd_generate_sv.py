#****************************************************************************
#* cmd_generate_sv.py
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
import argparse
from zuspec.gen import Output
from zuspec.cmd import CmdParseBase
import zsp_arl_dm.core as arl_dm

class CmdGenerateSv(arl_dm.VisitorBase,CmdParseBase):

    def __init__(self):
        super().__init__()
        pass

    def __call__(self, args):
        print("__call__")
        ctxt = self.parseFilesToCtxt(args.files)
        pss_top = ctxt.findDataTypeComponent("pss_top")
        print("pss_top: %s" % str(pss_top))

        for f in ctxt.getDataTypeFunctions():
            print("Function: %s" % f.name())
            self.visit(f)

        out = Output()
        out.println("Hello World")
        print("Result: %s" % out.getvalue())
        pass

    def visitDataTypeFunction(self, t):
        print("visitDataTypeFunction: %s" % t.name())

    @staticmethod
    def addGenerateSvCmd(name, subparsers):
        gen_cmd = subparsers.add_parser("generate-sv",
                    help="Generate SystemVerilog interface files")
        gen_cmd.set_defaults(func=CmdGenerateSv())

        gen_cmd.add_argument("-o", "--output",
            help="Specifies output file", default="pss_sv_if.svh")

        CmdParseBase.addFileArgs(gen_cmd)

        return gen_cmd

