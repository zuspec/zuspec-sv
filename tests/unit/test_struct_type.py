import os
import pytest
from .simple_test_flow import run_unit_test
from .sim_util import sim_dvflow as dvflow

def test_struct_comp_field(dvflow):
    content = """
        import std_pkg::*;
        struct S {
            int a, b;
        }
        component pss_top {
            S s1, s2;

            exec init_down {
                s1.a = 1;
                s1.b = 2;
            }
            action Entry {
                exec post_solve {
                    print("RES: a=%d b=%d\\n", comp.s1.a, comp.s1.b);
                }
            }
        }
    """
    expect = """
    RES: a=1 b=2
    """
    run_unit_test(dvflow, content, expect)

def test_struct_action_field(dvflow):
    content = """
        import std_pkg::*;
        struct S {
            int a, b;
        }
        component pss_top {

            exec init_down {
            }
            action Entry {
                S s1, s2;
                exec pre_solve {
                    s1.a = 1;
                    s1.b = 2;
                }

                exec post_solve {
                    print("RES: a=%d b=%d\\n", s1.a, s1.b);
                }
            }
        }
    """
    expect = """
    RES: a=1 b=2
    """
    run_unit_test(dvflow, content, expect)


def test_struct_exec_var(dvflow):
    content = """
        import std_pkg::*;
        struct S {
            int a, b;
        }
        component pss_top {

            exec init_down {
            }
            action Entry {

                exec post_solve {
                    S s1, s2;
                    s1.a = 1;
                    s1.b = 2;
                    print("RES: a=%d b=%d\\n", s1.a, s1.b);
                }
            }
        }
    """
    expect = """
    RES: a=1 b=2
    """
    run_unit_test(dvflow, content, expect)

def test_struct_assign(dvflow):
    content = """
        import addr_reg_pkg::*;
        import std_pkg::*;
        struct S {
            int a, b;
            addr_handle_t addr;
            int c;
        }
        component pss_top {

            exec init_down {
            }
            action Entry {

                exec post_solve {
                    S s1, s2;
                    s1.a = 1;
                    s1.b = 2;
                    s2 = s1;
                    s1.a = 3;
                    s1.b = 4;
                    print("RES: a=%d b=%d\\n", s1.a, s1.b);
                    print("RES: a=%d b=%d\\n", s2.a, s2.b);
                }
            }
        }
    """
    expect = """
    RES: a=3 b=4
    RES: a=1 b=2
    """
    run_unit_test(dvflow, content, expect)