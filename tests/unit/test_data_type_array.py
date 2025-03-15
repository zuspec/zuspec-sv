import os
import pytest
import sys
from .simple_test_flow import run_unit_test
from .sim_util import sim_dvflow as dvflow

def test_decl_int_array_comp(dvflow):
    content = """
        import std_pkg::*;
        component pss_top {
            int     arr[4];

            exec init_down {
                arr[0] = 1;
                arr[1] = 2;
                arr[2] = 3;
                arr[3] = 4;
            }

            action Entry {
                exec post_solve {
                    print("RES: arr=%d,%d,%d,%d\\n", 
                        comp.arr[0], comp.arr[1], 
                        comp.arr[2], comp.arr[3]);
                }
            }
        }
    """
    expect = """
    RES: arr=1,2,3,4
    """
    run_unit_test(dvflow, content, expect)

@pytest.mark.skip("Composite initializers not yet supported")
def test_decl_int_array_action(dvflow):
    content = """
        import std_pkg::*;
        component pss_top {

            action Entry {
                int     arr[4] = {1, 2, 3, 4};
                exec post_solve {
                    print("RES: arr=%d,%d,%d,%d\\n", 
                        arr[0], arr[1], 
                        arr[2], arr[3]);
                }
            }
        }
    """
    expect = """
    RES: arr=1,2,3,4
    """
    run_unit_test(dvflow, content, expect)

def test_decl_rand_int_array_action(dvflow):
    content = """
        import std_pkg::*;
        component pss_top {

            action Entry {
                rand int     arr[4];
                constraint {
                    arr[0] == 1;
                    arr[1] == 2;
                    arr[2] == 3;
                    arr[3] == 4;
                }
                exec post_solve {
                    print("RES: arr=%d,%d,%d,%d\\n", 
                        arr[0], arr[1], 
                        arr[2], arr[3]);
                }
            }
        }
    """
    expect = """
    RES: arr=1,2,3,4
    """
    if dvflow.sim == "vlt":
        pytest.skip("Nested randomization not supported as of 5.028")
    else:
        run_unit_test(dvflow, content, expect)

def test_decl_rand_int_array_action_foreach(dvflow):
    content = """
        import std_pkg::*;
        component pss_top {

            action Entry {
                rand int     arr[4];
                constraint {
                    foreach (arr[i]) {
                        arr[i] == (i+1);
                    }
                }
                exec post_solve {
                    print("RES: arr=%d,%d,%d,%d\\n", 
                        arr[0], arr[1], 
                        arr[2], arr[3]);
                }
            }
        }
    """
    expect = """
    RES: arr=1,2,3,4
    """
    if dvflow.sim == "vlt":
        pytest.skip("Nested randomization not supported as of 5.028")
    else:
        run_unit_test(dvflow, content, expect)

