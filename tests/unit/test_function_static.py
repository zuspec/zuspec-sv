import os
import pytest
from .simple_test_flow import run_unit_test
from .sim_util import sim_dvflow as dvflow


def test_function_global(dvflow):
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
    run_unit_test(dvflow, content, expect, debug=False)

def test_function_global_rval_int(dvflow):
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
    run_unit_test(dvflow, content, expect, debug=False)

def test_function_global_recurse(dvflow):
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

    if dvflow.sim != "vlt":
        run_unit_test(dvflow, content, expect, debug=False)
    else:
        print("SKIP: test_function_global_recurse on vlt")
