import os
import pytest
from .simple_test_flow import run_unit_test
from .sim_util import sim_dvflow as dvflow

def test_eq(dvflow):
    content = """
        import std_pkg::*;
        component pss_top {
            action Entry {
                rand bit[4] a, b;

                constraint ab_c {
                    a == 1;
                    b == 2;
                }
                exec post_solve {
                    print("RES: a=%d\\n", a);
                    print("RES: b=%d\\n", b);
                }
            }
        }
    """
    expect = """
    RES: a=1
    RES: b=2
    """
    run_unit_test(dvflow, content, expect)

def test_if_1(dvflow):
    content = """
        import std_pkg::*;
        component pss_top {
            action Entry {
                bit[32] c1 = 1;
                rand bit[4] a;

                constraint ab_c {
                    if (c1 == 1) {
                        a == 1;
                    } else {
                        a == 2;
                    }
                }
                exec post_solve {
                    print("RES: a=%d\\n", a);
                }
            }
        }
    """
    expect = """
    RES: a=1
    """
    run_unit_test(dvflow, content, expect)

def test_if(dvflow):
    content = """
        import std_pkg::*;
        component pss_top {
            action Entry {
                bit[32] c1 = 1;
                bit[32] c2 = 2;
                rand bit[4] a, b;

                constraint ab_c {
                    if (c1 == 1) {
                        a == 1;
                    } else {
                        a == 2;
                    }
                    if (c2 == 1) {
                        b == 1;
                    } else {
                        b == 2;
                    }
                }
                exec post_solve {
                    print("RES: a=%d\\n", a);
                    print("RES: b=%d\\n", b);
                }
            }
        }
    """
    expect = """
    RES: a=1
    RES: b=2
    """
    run_unit_test(dvflow, content, expect)

# Appears to be a Verilator issue for now
# def test_implies(dvflow):
#     content = """
#         import std_pkg::*;
#         component pss_top {
#             action Entry {
#                 bit[32] c1 = 1;
#                 bit[32] c2 = 2;
#                 rand bit[4] a, b;

#                 constraint ab_c {
#                     (c1 == 1) -> a == 1;
#                     (c2 == 2) -> b == 2;
#                 }
#                 exec post_solve {
#                     print("RES: a=%d\\n", a);
#                     print("RES: b=%d\\n", b);
#                 }
#             }
#         }
#     """
#     expect = """
#     RES: a=1
#     RES: b=2
#     """
#     run_unit_test(dvflow, content, expect)