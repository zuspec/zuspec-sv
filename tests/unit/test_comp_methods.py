import os
import pytest
import sys
from .simple_test_flow import run_unit_test
from .sim_util import sim_dvflow as dvflow

def test_call_comp_method(dvflow):
    content = """
        import std_pkg::*;
        component pss_top {
            function void doit() {
                print("RES: Hello World\\n");
            }

            action Entry {
                exec post_solve {
                    comp.doit();
                }
            }
        }
    """
    expect = """
    RES: Hello World
    """
    run_unit_test(
        dvflow, 
        content, 
        expect,
        "top_default_api.sv",
        debug=False)

def test_call_comp_global_method(dvflow):
    content = """
        import std_pkg::*;
        function void doit_e() {
            print("RES: Hello World\\n");
        }

        component pss_top {
            function void doit() {
                doit_e();
            }

            action Entry {
                exec post_solve {
                    comp.doit();
                }
            }
        }
    """
    expect = """
    RES: Hello World
    """
    run_unit_test(
        dvflow, 
        content, 
        expect,
        "top_default_api.sv",
        debug=False)

def test_call_comp_global_qmethod(dvflow):
    content = """
        import std_pkg::*;
        function void doit_e() {
            print("RES: Hello World\\n");
        }

        component pss_top {
            function void doit() {
                ::doit_e();
            }

            action Entry {
                exec post_solve {
                    comp.doit();
                }
            }
        }
    """
    expect = """
    RES: Hello World
    """
    run_unit_test(
        dvflow, 
        content, 
        expect,
        "top_default_api.sv",
        debug=False)
    
