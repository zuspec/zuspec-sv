import os
import pytest
import pytest_fv as pfv
from pytest_fv.fixtures import *
import sys
from .simple_test_flow import run_unit_test

def test_call_comp_method(dirconfig):
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
        dirconfig, 
        content, 
        expect,
        "top_default_api.sv",
        debug=False)

def test_call_comp_global_method(dirconfig):
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
        dirconfig, 
        content, 
        expect,
        "top_default_api.sv",
        debug=False)

def test_call_comp_global_qmethod(dirconfig):
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
        dirconfig, 
        content, 
        expect,
        "top_default_api.sv",
        debug=False)
    
