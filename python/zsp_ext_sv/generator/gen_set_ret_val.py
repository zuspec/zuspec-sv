#****************************************************************************
#* gen_set_ret_val.py
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
import zsp_arl_dm.core as arl_dm

class GenSetRetVal(arl_dm.VisitorBase):

    def __init__(self, out):
        super().__init__()
        self._out = out

    def gen(self, dt):
        self.visit(dt)

    def visitDataTypeInt(self, i):
        self._out.println("thread.setIntResult(__retval, %s, %d);" % (
            "1" if i.is_signed() else "0",
            i.width()
        ))

    def visitDataTypeStruct(self, i):
        raise Exception("Returning a struct type is not supported")
