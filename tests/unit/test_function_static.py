import os
import pytest
import pytest_fv as pfv
from pytest_fv.fixtures import *
import sys
from .simple_test_flow import run_unit_test


def test_function_global(dirconfig):
    content = """
        import std_pkg::*;
        function void print_val(int v) {
          print("RES: %d\\n", v);
        }
        component pss_top {
            action Entry {
                exec post_solve {
                    print_val(2);
                    print_val(4);
                }
            }
        }
    """
    expect = """
    RES: 2
    RES: 4
    """
    run_unit_test(dirconfig, content, expect, debug=True)

def test_function_global_rval_int(dirconfig):
    content = """
        import std_pkg::*;
        function int print_val(int v) {
          return v+2;
        }
        component pss_top {
            action Entry {
                exec post_solve {
                    print("RES: %d\\n", print_val(2));
                    print("RES: %d\\n", print_val(4));
                    print_val(4);
                }
            }
        }
    """
    expect = """
    RES: 4
    RES: 6
    """
    run_unit_test(dirconfig, content, expect, debug=True)

def test_function_global_recurse(dirconfig):
    content = """
        import std_pkg::*;
        function int fibonacci(int n) {
            if (n < 2) {
                return 1;
            } else {
                return fibonacci(n-2) + fibonacci(n-1);
            }
        }
        component pss_top {
            action Entry {
                exec post_solve {
                    print("RES: %d\\n", fibonacci(0));
                    print("RES: %d\\n", fibonacci(1));
                    print("RES: %d\\n", fibonacci(2));
                    print("RES: %d\\n", fibonacci(3));
                    print("RES: %d\\n", fibonacci(4));
                }
            }
        }
    """
    expect = """
    RES: 1
    RES: 1
    RES: 2
    RES: 3
    RES: 5
    """

    if dirconfig.config.getHdlSim() != "vlt":
        run_unit_test(dirconfig, content, expect, debug=True)
    else:
        print("SKIP: test_function_global_recurse on vlt")
