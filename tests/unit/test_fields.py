import os
import pytest
from .simple_test_flow import run_unit_test
from .sim_util import sim_dvflow as dvflow


def test_bool_field(dvflow):
    content = """
        import std_pkg::*;
        component pss_top {
            action Entry {
                rand bool f1, f2;
                constraint f1 == 0;
                constraint f2 == 1;
                exec post_solve {
                    print("RES: f1=%d\\n", f1);
                    print("RES: f2=%d\\n", f2);
                }
            }
        }
    """
    expect = """
    RES: f1=0
    RES: f2=1
    """
    run_unit_test(dvflow, content, expect)

def test_bit_field(dvflow):
    content = """
        import std_pkg::*;
        component pss_top {
            action Entry {
                rand bit[4] f1;
                constraint f1 == 15;
                exec post_solve {
                    print("RES: f1=%d\\n", f1);
                }
            }
        }
    """
    expect = """
    RES: f1=15
    """
    run_unit_test(dvflow, content, expect)

def test_int_field(dvflow):
    content = """
        import std_pkg::*;
        component pss_top {
            action Entry {
                rand int[4] f1;
                constraint f1 == 15;
                exec post_solve {
                    print("RES: f1=%d\\n", f1);
                }
            }
        }
    """
    expect = """
    RES: f1=-1
    """
    run_unit_test(dvflow, content, expect)

def disabled_test_struct_field(dvflow):
    content = """
        import std_pkg::*;
        struct S {
            rand bit[4] a;
        }
        component pss_top {
            action Entry {
                rand S f1;
                constraint f1.a == 4;
                exec post_solve {
                    print("RES: f1.a=%d\\n", f1.a);
                }
            }
        }
    """
    expect = """
    RES: f1.a=4
    """
    run_unit_test(dvflow, content, expect)

def disabled_test_struct_nested_field(dvflow):
    content = """
        import std_pkg::*;
        struct S1 {
            rand bit[4] a;
            exec pre_solve {
                print("RES: S1.pre_solve\\n");
            }
            exec post_solve {
                print("RES: S1.post_solve\\n");
            }
        }
        struct S {
            rand bit[4] a;
            rand S1     b;
            exec pre_solve {
                print("RES: S.pre_solve\\n");
            }
            exec post_solve {
                print("RES: S.post_solve\\n");
            }
        }
        component pss_top {
            action Entry {
                rand S f1;
                exec pre_solve {
                    print("RES: Entry.pre_solve\\n");
                }
                constraint f1.a == 4;
                constraint f1.b.a == 7;
                exec post_solve {
                    print("RES: f1.a=%d f1.b.a=%d\\n", f1.a, f1.b.a);
                }
            }
        }
    """
    expect = """
    RES: Entry.pre_solve
    RES: S.pre_solve
    RES: S1.pre_solve
    RES: f1.a=4 f1.b.a=7
    RES: S.post_solve
    RES: S1.post_solve
    """
    run_unit_test(dvflow, content, expect)
