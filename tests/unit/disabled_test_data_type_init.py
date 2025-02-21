import os
import pytest
import pytest_fv as pfv
from pytest_fv.fixtures import *
import sys
from .simple_test_flow import run_unit_test


def test_init_struct(dirconfig):
    content = """
        import std_pkg::*;
        struct MyS {
            int a, b, c, d;
        }
        component pss_top {
            action Entry {
                exec post_solve {
                    MyS s = {.a=4, .b=6, .c=7};
                    s = {.a=4, .b=6, .c=7};
                    print("RES: a=%d b=%d", s.a, s.b);
                }
            }
        }
    """
    expect = """
    RES: a=4 b=6
    """
    run_unit_test(dirconfig, content, expect, debug=False)