#****************************************************************************
#* test_smoke.py
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
from .test_base import TestBase

class TestSmoke(TestBase):

    def test_smoke(self):
        content = """
        component pss_top {
            action Entry {
                exec post_solve {
//                    print("Hello World!");
                }
            }
        }
        """

        ctxt, comp_t, action_t = self.buildModelGetRoots(
            content,
            "pss_top",
            "pss_top::Entry")

        out = io.StringIO() 
        generator = self.zsp_sv_f.mkGenerateExecActor(
            ctxt,
            comp_t,
            action_t,
            out)
        generator.generate()

        print("Output:\n%s" % out.getvalue())

    def test_smoke_nested(self):
        content = """
        component Sub {
            exec init_down {
                int a;
                int b;
            }
        }

        component pss_top {
            Sub c1, c2;
            action Entry {
                rand bit[4] a, b, c;

                constraint ab_c {
                    a < b;
                }

                constraint c > b;

                exec post_solve {
//                    print("Hello World!");
                }
            }
        }
        """

        ctxt, comp_t, action_t = self.buildModelGetRoots(
            content,
            "pss_top",
            "pss_top::Entry")

        out = io.StringIO() 
        generator = self.zsp_sv_f.mkGenerateExecActor(
            ctxt,
            comp_t,
            action_t,
            out)
        generator.generate()

        print("Output:\n%s" % out.getvalue())

    def test_smoke_constraint(self):
        content = """
        component pss_top {
            action Entry {
                rand bit[4] a;

                constraint ab_c {
                    a != 0;
                }
            }
        }
        """

        ctxt, comp_t, action_t = self.buildModelGetRoots(
            content,
            "pss_top",
            "pss_top::Entry",
            load_stdlib=False)

        out = io.StringIO() 
        generator = self.zsp_sv_f.mkGenerateExecActor(
            ctxt,
            comp_t,
            action_t,
            out)
        generator.generate()

        print("Output:\n%s" % out.getvalue())

    def test_smoke_print(self):
        content = """
        import std_pkg::*;
        component pss_top {
            action Entry {
                rand bit[4] a;

                exec post_solve {
                    print("Hello World! %d", a);
                }

                constraint ab_c {
                    a != 0;
                }
            }
        }
        """

        ctxt, comp_t, action_t = self.buildModelGetRoots(
            content,
            "pss_top",
            "pss_top::Entry",
            load_stdlib=True)

        out = io.StringIO() 
        generator = self.zsp_sv_f.mkGenerateExecActor(
            ctxt,
            comp_t,
            action_t,
            out)
        generator.generate()

        print("Output:\n%s" % out.getvalue())
