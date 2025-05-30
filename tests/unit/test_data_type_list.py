import os
import pytest
from .simple_test_flow import run_unit_test
from .sim_util import sim_dvflow as dvflow

def test_decl_int_list_comp(dvflow):
    content = """
        import std_pkg::*;
        component pss_top {
            list<int> arr;

            exec init_down {
                arr.push_back(1);
                arr.push_back(2);
                arr.push_back(3);
                arr.push_back(4);
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

@pytest.mark.skip("exec foreach not yet supported")
def test_decl_int_list_comp_foreach(dvflow):
    content = """
        import std_pkg::*;
        component pss_top {
            list<int> arr;

            exec init_down {
                repeat(i : 4) {
                    arr.push_back(i);
                }
                foreach (arr[i]) {
                    arr[i] = arr[i] + 1;
                }
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

@pytest.mark.skip("composite initializers not supported yet")
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

def test_decl_rand_int_list_action(dvflow):
    content = """
        import std_pkg::*;
        component pss_top {

            action Entry {
                rand list<int> arr;
                exec pre_solve {
                    repeat (4) {
                        arr.push_back(0);
                    }
                }
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
        pytest.skip("vlt doesn't support nested randomization as of 5.028")
    else:
        run_unit_test(dvflow, content, expect)

def test_decl_rand_int_list_action_foreach(dvflow):
    content = """
        import std_pkg::*;
        component pss_top {

            action Entry {
                rand list<int>     arr;
                exec pre_solve {
                    repeat(4) {
                        arr.push_back(0);
                    }
                }

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
        pytest.skip("vlt as of 5.028 does not support foreach")
    else:
        run_unit_test(dvflow, content, expect)

