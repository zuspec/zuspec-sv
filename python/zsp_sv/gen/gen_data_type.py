#****************************************************************************
#* gen_data_type.py
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
import zsp_arl_dm.core as arl_dm

class GenDataType(arl_dm.VisitorBase):

    def __init__(self, out):
        super().__init__()
        self._out = out
        pass

    def gen(self, dt):
        self.visit(dt)

    def visitDataTypeAddrHandle(self, i):
        self.write("longint unsigned")

    def visitDataTypeInt(self, i):
        width = i.width()
        is_signed = i.is_signed()

        if width > 32:
            self.write("longint%s" % (" unsigned" if not is_signed else "",))
        elif width > 16:
            self.write("int%s" % (" unsigned" if not is_signed else "",))
        elif width > 8:
            self.write("shortint%s" % (" unsigned" if not is_signed else "",))
        else:
            self.write("byte%s" % (" unsigned" if not is_signed else "",))

    def visitDataTypeStruct(self, i):
        xlate_m = {
            "addr_reg_pkg::addr_handle_t" : "longint unsigned"
        }
        if i.name() in xlate_m.keys():
            self.write("%s" % xlate_m[i.name()])
        else:
            self.write("%s" % i.name())

    def write(self, s):
        self._out.write(s)

    def println(self, s):
        self._out.println(s)

    def inc_ind(self):
        self._out.inc_ind()

    def dec_ind(self):
        self._out.dec_ind()

