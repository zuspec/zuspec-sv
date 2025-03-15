import os
import pytest
import sys
from .simple_test_flow import run_unit_test
from .sim_util import sim_dvflow as dvflow

def test_single_level_init(dvflow):
    content = """
        import std_pkg::*;
        component pss_top {
            exec init_down {
                print("RES: init_down\\n");
            }
            exec init_up {
                print("RES: init_up\\n");
            }
            action Entry {
            }
        }
    """
    expect = """
    RES: init_down
    RES: init_up
    """
    run_unit_test(dvflow, content, expect)

def test_nested_dual(dvflow):
    content = """
        import std_pkg::*;
        component C {
            exec init_down {
                print("RES: C.init_down\\n");
            }
            exec init_up {
                print("RES: C.init_up\\n");
            }
        }
        component pss_top {
            C c1, c2;
            exec init_down {
                print("RES: init_down\\n");
            }
            exec init_up {
                print("RES: init_up\\n");
            }
            action Entry {
            }
        }
    """
    expect = """
    RES: init_down
    RES: C.init_down
    RES: C.init_up
    RES: C.init_down
    RES: C.init_up
    RES: init_up
    """
    run_unit_test(dvflow, content, expect)
