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
from .gen_set_ret_val import GenSetRetVal
from zuspec.gen import Output
from zuspec.cli import CmdParseBase
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
            "addr_reg_pkg::reg_group_c::get_offset_of_instance",
            "addr_reg_pkg::reg_group_c::get_offset_of_instance_array",
            "addr_reg_pkg::reg_group_c::get_offset_of_path",
            "addr_reg_pkg::addr_value",
            "executor_pkg::executor",
            "std_pkg::print",
        }
        self.out = None
        self.phase = None
        self.method_id_name_m = {}
        self.method_name_id_m = {}
        pass

    def __call__(self, args):
        self.ctxt = self.parseFilesToCtxt(args.files)

        self.method_id_name_m = {}
        self.method_name_id_m = {}

        fp = open(args.output, "w")

        self.out = Output(fp)
        self.out.println("package pss_api_pkg;")
        self.out.inc_ind()
        self.out.println("")

        self.out.println("class EmptyBase;")
        self.out.inc_ind()
        self.out.println("function new(string name=\"\");")
        self.out.println("endfunction")
        self.out.dec_ind()
        self.out.println("endclass")
        self.out.println()

        en_functions = []

        for f in self.ctxt.getDataTypeFunctions():
            print("Function: %s" % f.name(), flush=True)
            if not self.ignore_func(f):
                print("  Add to list", flush=True)
                en_functions.append(f)

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

            for f in en_functions:
                self.visit(f)

            self.out.dec_ind()

            self.out.println("endclass")
            self.out.println("")
        self.out.dec_ind()
        self.out.println("endpackage")
        self.out.println("")
        self.out.println("")

        self.out.println("package zuspec_actor_pkg;")
        self.out.inc_ind()
        self.out.println("import zuspec::*;")
        self.out.println("import pss_api_pkg::*;")
        self.out.println("")
        
        self.phase = Phase.Actor
        self.out.println("class Actor #(type TARGET_T=PssIF) extends MethodBridge;")
        self.out.inc_ind()
        self.out.println("ActorCore             m_core;")
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
        self.out.println("virtual function void init(ActorCore actor);")
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

        self.out.println("task run();")
        self.out.inc_ind()
        self.out.println("m_core.run();")
        self.out.dec_ind()
        self.out.println("endtask")
        self.out.println("")
        self.out.println("")

        self.out.println("virtual task invokeFuncTarget(")
        self.out.inc_ind()
        self.out.println("zuspec::EvalThread     thread,")
        self.out.println("int                    func_id,")
        self.out.println("zuspec::ValRef         params[]);")
        self.out.println("case (func_id)")
        self.out.inc_ind()
        for f in self.ctxt.getDataTypeFunctions():
            if f.name() not in self.method_name_id_m.keys():
                continue
            if f.hasFlags(arl_dm.DataTypeFunctionFlags.Solve):
                continue
            # TODO: task vs function
            self.out.println("%d: begin // %s" % (
                self.method_name_id_m[f.name()],
                f.name()))
            self.out.inc_ind()
            self.visit(f)
            self.out.dec_ind()
            self.out.println("end")
        self.out.println("default: begin")
        self.out.inc_ind()
        self.out.println("$display(\"FATAL: unsupported function id %d\", func_id);")
        self.out.println("$finish;")
        self.out.dec_ind()
        self.out.println("end")

        self.out.dec_ind()
        self.out.println("endcase")

        self.out.dec_ind()
        self.out.println("endtask")

        self.out.println("virtual function void invokeFuncSolve(")
        self.out.inc_ind()
        self.out.println("zuspec::EvalThread     thread,")
        self.out.println("int                    func_id,")
        self.out.println("zuspec::ValRef         params[]);")
        self.out.println("case (func_id)")
        self.out.inc_ind()
        for f in self.ctxt.getDataTypeFunctions():
            if f.name() not in self.method_name_id_m.keys():
                continue
            if not f.hasFlags(arl_dm.DataTypeFunctionFlags.Solve):
                continue

            # TODO: task vs function
            self.out.println("%d: begin // %s" % (
                self.method_name_id_m[f.name()],
                f.name()))
            self.out.inc_ind()
            self.visit(f)
            self.out.dec_ind()
            self.out.println("end")

        self.out.println("default: begin")
        self.out.inc_ind()
        self.out.println("$display(\"FATAL: unsupported function id %d\", func_id);")
        self.out.println("$finish;")
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

        fp.close()

    def visitDataTypeAction(self, t):
        pass

    def visitDataTypeStruct(self, t):
        pass

    def visitDataTypeComponent(self, t):
        pass

    def visitDataTypeFunction(self, t):
        print("Name: %s" % t.name(), flush=True)
        if self.phase in (Phase.BaseIF, Phase.PureIF):
            self.emitFuncDecl(t)
        elif self.phase == Phase.Actor:
            self.emitFuncCall(t)

    def ignore_func(self, t : arl_dm.DataTypeFunction):
        ignore_elems = { "reg_group_c", "transparent_addr_space_c" }
        ignore_prefs = { 'contiguous_addr_space_c<', 'transparent_addr_space_c<', 'reg_c<' }

        if t.name() in self.ignore_funcs:
            return True
        
        elems = t.name().split("::")
        
        for elem in elems:
            print("Check elem \"%s\"" % elem, flush=True)
            if elem in ignore_elems:
                return True
            
            for pref in ignore_prefs:
                if elem.startswith(pref):
                    return True


            dt = self.ctxt.findDataTypeStruct(elem)

            if dt is not None:
                return True

        return False
    
    def emitFuncDecl(self, t):
        if self.ignore_func(t):
            return
        name = t.name()

#        print("Function: %s" % name, flush=True)

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
        is_target = not t.hasFlags(arl_dm.DataTypeFunctionFlags.Solve) # TODO:

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
            GenSetRetVal(self.out).gen(t.getReturnType())
        else:
            self.out.println("thread.setVoidResult();")

    @staticmethod
    def addGenerateSvCmd(name, subparsers):
        gen_cmd = subparsers.add_parser("gen-sv-import-api",
                    help="Generate SystemVerilog class API for PSS import functions")
        gen_cmd.set_defaults(func=CmdGenerateSv())

        gen_cmd.add_argument("-o", "--output",
            help="Specifies output file", default="pss_api_pkg.sv")

        CmdParseBase.addFileArgs(gen_cmd)

        return gen_cmd

