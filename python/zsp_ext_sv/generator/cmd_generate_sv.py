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
from enum import Enum, auto
from .gen_data_type import GenDataType
from .gen_get_ref_val import GenGetRefVal
from zuspec.gen import Output
from zuspec.cmd import CmdParseBase
import zsp_arl_dm.core as arl_dm

class Phase(Enum):
    PureIF = auto()
    BaseIF = auto()
    Actor  = auto()

class CmdGenerateSv(arl_dm.VisitorBase,CmdParseBase):

    def __init__(self):
        super().__init__()
        self.ignore_funcs = {
            "addr_reg_pkg::make_handle_from_claim",
            "addr_reg_pkg::make_handle_from_handle",
            "addr_reg_pkg::addr_value",
            "executor_pkg::executor",
        }
        self.out = None
        self.phase = None
        self.method_id_name_m = {}
        self.method_name_id_m = {}
        pass

    def __call__(self, args):
        print("__call__")
        ctxt = self.parseFilesToCtxt(args.files)
        pss_top = ctxt.findDataTypeComponent("pss_top")
        print("pss_top: %s" % str(pss_top))

        self.method_id_name_m = {}
        self.method_name_id_m = {}

        self.out = Output()
        self.out.println("package pss_api_pkg;")
        self.out.inc_ind()
        self.out.println("import zuspec::*;")
        self.out.println("")

        self.out.println("class EmptyBase;")
        self.out.inc_ind()
        self.out.println("function new(string name=\"\");")
        self.out.println("endfunction")
        self.out.dec_ind()
        self.out.println("endclass")
        self.out.println()

        for phase in (Phase.PureIF, Phase.BaseIF):
            self.phase = phase

            if phase == Phase.PureIF:
                self.out.println("interface class PssIF;")
            else:
                self.out.println("class PssBaseIF #(type BASE_T=EmptyBase) extends BASE_T implements PssIF;")
            
            self.out.inc_ind()

            if phase == Phase.BaseIF:
                self.out.println("function new(string name=\"\");")
                self.out.inc_ind()
                self.out.println("super.new(name);")
                self.out.dec_ind()
                self.out.println("endfunction")

            for f in ctxt.getDataTypeFunctions():
                print("Function: %s" % f.name())
                self.visit(f)

            self.out.dec_ind()

            self.out.println("endclass")
            self.out.println("")
        
        self.phase = Phase.Actor
        self.out.println("class Actor #(type TARGET_T=PssIF) extends MethodBridge;")
        self.out.inc_ind()
        self.out.println("zuspec::Actor         m_core;")
        self.out.println("TARGET_T              m_targets[];")
        self.out.println("")
        self.out.println("function new(")
        self.out.inc_ind()
        self.out.println("string     comp_t,")
        self.out.println("string     action_t,")
        self.out.println("TARGET_T   targets[]")
        self.out.dec_ind()
        self.out.println(");")
        self.out.inc_ind()
        self.out.println("m_targets = new[targets.size()](targets);")
        self.out.println("m_core    = new(comp_t, action_t, this);")
        self.out.dec_ind()
        self.out.println("endfunction")
        self.out.println("")
        self.out.println("virtual function init(zuspec::Actor actor);")
        self.out.inc_ind()
        self.out.println("super.init(actor);")
        for i in range(len(self.method_id_name_m)):
            name = self.method_id_name_m[i]
            self.out.println("if (!actor.registerFunctionId(\"%s\", %d)) begin" % (name, i))
            self.out.inc_ind()
            self.out.println("$display(\"FATAL: Failed to register PSS function %s\");" % name)
            self.out.println("$finish;")
            self.out.dec_ind()
            self.out.println("end")
        self.out.dec_ind()
        self.out.println("endfunction")
        self.out.println()

        self.out.println("virtual task invokeFuncTarget(")
        self.out.inc_ind()
        self.out.println("zuspec::EvalThread     thread,")
        self.out.println("int                    func_id,")
        self.out.println("zuspec::ValRef         params[]);")
        self.out.println("case (func_id)")
        self.out.inc_ind()
        for f in ctxt.getDataTypeFunctions():
            if f.name() not in self.method_name_id_m.keys():
                continue
            # TODO: task vs function
            self.out.println("%d: begin // %s" % (
                self.method_name_id_m[f.name()],
                f.name()))
            self.out.inc_ind()
            self.visit(f)
            self.out.dec_ind()
            self.out.println("end")
        self.out.dec_ind()
        self.out.println("endcase")

        self.out.dec_ind()
        self.out.println("endtask")

        self.out.println("virtual function invokeFuncSolve(")
        self.out.inc_ind()
        self.out.println("zuspec::EvalThread     thread,")
        self.out.println("int                    func_id,")
        self.out.println("zuspec::ValRef         params[]);")
        self.out.println("case (func_id)")
        self.out.inc_ind()
        for f in ctxt.getDataTypeFunctions():
            if f.name() not in self.method_name_id_m.keys():
                continue
            # TODO: task vs function
            self.out.println("%d: begin // %s" % (
                self.method_name_id_m[f.name()],
                f.name()))
            self.out.inc_ind()
            self.visit(f)
            self.out.dec_ind()
            self.out.println("end")
        self.out.dec_ind()
        self.out.println("endcase")

        self.out.dec_ind()
        self.out.println("endfunction")

        self.out.println()
        self.out.dec_ind()
        self.out.println("endclass")

        self.out.dec_ind()
        self.out.println("endpackage")

        print("Result:\n%s" % self.out.getvalue())
        pass

    def visitDataTypeFunction(self, t):
        if self.phase in (Phase.BaseIF, Phase.PureIF):
            self.emitFuncDecl(t)
        elif self.phase == Phase.Actor:
            self.emitFuncCall(t)
    
    def emitFuncDecl(self, t):
        if t.name() in self.ignore_funcs:
            return
        name = t.name()

        if name not in self.method_name_id_m.keys():
            sz = len(self.method_id_name_m)
            self.method_name_id_m[name] = sz
            self.method_id_name_m[sz] = name

        colon_idx = name.rfind("::")
        if colon_idx != -1:
            name = name[colon_idx+2:]

        self.out.println("%svirtual task %s(" % (
            "pure " if self.phase == Phase.PureIF else "",
            name))

        parameters = t.getParameters()
        self.out.push_comma(len(parameters))
        self.out.inc_ind()

        if t.getReturnType() is not None:
            self.out.write("%soutput " % self.out.ind())
            GenDataType(self.out).gen(t.getReturnType())
            self.out.write(" retval%s\n" % self.out.comma())
        
        self.out.pop_comma()

        for i,p in enumerate(parameters):
            self.out.write("%sinput " % self.out.ind())
            GenDataType(self.out).gen(p.getDataType())
            self.out.write(" %s%s\n" % (p.name(), "," if i+1 < len(parameters) else ""))
        self.out.dec_ind()
        self.out.println(");")

        if self.phase == Phase.BaseIF:
            self.out.inc_ind()
            self.out.println("$display(\"FATAL: Function %s not implemented\");" % name)
            self.out.println("$finish;")
            self.out.dec_ind()
            self.out.println("endtask")

    def emitFuncCall(self, t):
        is_target = False # TODO:

        # Declare temp variables for parameters
        parameters = t.getParameters()

        if t.getReturnType() is not None:
            self.out.write(self.out.ind())
            GenDataType(self.out).gen(t.getReturnType())
            self.out.write(" __retval;\n")

        for i,p in enumerate(parameters):
            self.out.write(self.out.ind())
            GenDataType(self.out).gen(p.getDataType())
            self.out.write(" __%s = " % p.name())
            GenGetRefVal(self.out).gen(p.getDataType(), "params[%d]" % i)
            self.out.write(";\n")

        name = t.name()

        colon_idx = name.rfind("::")
        if colon_idx != -1:
            name = name[colon_idx+2:]

        self.out.write(self.out.ind())
        if not is_target and t.getReturnType() is not None:
            self.out.write("__ret = ")
        self.out.write("m_targets[0].%s(\n" % name)
        self.out.inc_ind()

        if is_target and t.getReturnType() is not None:
            self.out.println("__retval%s" % ("," if len(parameters) > 0 else "",))

        for i,p in enumerate(parameters):
            self.out.println("__%s%s" % (p.name(),
                                     "," if (i+1) < len(parameters) else ""))
        self.out.dec_ind()
        self.out.println(");")
        if t.getReturnType() is not None:
            pass
        else:
            self.out.println("thread.setVoidResult();")

        pass


    @staticmethod
    def addGenerateSvCmd(name, subparsers):
        gen_cmd = subparsers.add_parser("generate-sv",
                    help="Generate SystemVerilog interface files")
        gen_cmd.set_defaults(func=CmdGenerateSv())

        gen_cmd.add_argument("-o", "--output",
            help="Specifies output file", default="pss_sv_if.svh")

        CmdParseBase.addFileArgs(gen_cmd)

        return gen_cmd

