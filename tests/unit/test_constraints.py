import os
import pytest
import pytest_fv as pfv
from pytest_fv.fixtures import *
import sys
from .simple_test_flow import run_unit_test

def test_if(dirconfig):
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
                    print("RES: a=%d", a);
                    print("RES: b=%d", b);
                }
            }
        }
    """
    expect = """
    RES: a=1
    RES: b=2
    """
    run_unit_test(dirconfig, content, expect)

def test_implies(dirconfig):
    content = """
        import std_pkg::*;
        component pss_top {
            action Entry {
                bit[32] c1 = 1;
                bit[32] c2 = 2;
                rand bit[4] a, b;

                constraint ab_c {
                    (c1 == 1) -> a == 1;
                    (c2 == 2) -> b == 2;
                }
                exec post_solve {
                    print("RES: a=%d", a);
                    print("RES: b=%d", b);
                }
            }
        }
    """
    expect = """
    RES: a=1
    RES: b=2
    """
    run_unit_test(dirconfig, content, expect)