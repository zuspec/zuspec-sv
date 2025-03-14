import os
import pytest
import pytest_fv as pfv
from pytest_fv.fixtures import *
from pytest_dv import *
import sys
from .simple_test_flow import run_unit_test

def test_leaf_action(dvflow):
    content = """
import std_pkg::*;

component pss_top {
    action Entry {
        exec body {
            message(0, "RES: Hello World!");
        }
    }
}
    """

    expect = """
    RES: Hello World!
    """
    run_unit_test(dvflow, content, expect)

def test_subactivity(dirconfig):
    content = """
        import std_pkg::*;
        component pss_top {

            action Leaf {
                exec post_solve {
                    print("RES: leaf\\n");
                }
            }

            action Mid {
                activity {
                    do Leaf;
                    do Leaf;
                }
            }

            action Entry {
                activity {
                    do Mid;
                }
            }
        }
    """

    expect = """
    RES: leaf
    RES: leaf
    """
    run_unit_test(dirconfig, content, expect)

def test_subactivity_with(dirconfig):
    content = """
        import std_pkg::*;
        component pss_top {

            action Leaf {
                rand bit[16] val0, val;
                exec post_solve {
                    print("RES: leaf %d\\n", val);
                }
            }

            action Mid {
                int i;
                activity {
                    do Leaf with {
                      val == 1;
                    };
                    do Leaf with {
                      val == 2;
                    }
                }
            }

            action Entry {
                activity {
                    do Mid;
                }
            }
        }
    """

    expect = """
    RES: leaf 1
    RES: leaf 2
    """

    if dirconfig.config.getHdlSim() == "vlt":
        pytest.skip("Nested randomization not supported as of 5.028")
    else:
        run_unit_test(dirconfig, content, expect, debug=True)

def test_subactivity_subcomp(dirconfig):
    content = """
        import std_pkg::*;
        component C {
            int cv;
            action Leaf {
                rand bit[16] val0, val;
                exec post_solve {
                    print("RES: leaf %d\\n", val);
                }
            }

            action Mid {
                int i;
                activity {
                    do Leaf with {
                      val == 1;
                    };
                    do Leaf with {
                      val == 2;
                    }
                }
            }

        }
        component pss_top {
            C c1, c2, c3, c4;

            exec init_down {
                c1.cv = 1;
                c2.cv = 2;
                c3.cv = 3;
                c4.cv = 4;
            }

            action Entry {
                exec pre_solve {
                    print("Entry::pre_solve\\n");
                }

                activity {
                    do C::Mid;
                }
            }
        }
    """

    expect = """
    RES: leaf 1
    RES: leaf 2
    """
    if dirconfig.config.getHdlSim() == "vlt":
        pytest.skip("Nested randomization not supported as of 5.028")
    else:
        run_unit_test(dirconfig, content, expect)

def test_subactivity_listener(dirconfig):
    content = """
        import std_pkg::*;
        component pss_top {

            action Leaf {
                exec post_solve {
                    print("RES: leaf\\n");
                }
            }

            action Mid {
                activity {
                    do Leaf;
                    do Leaf;
                }
            }

            action Entry {
                activity {
                    do Mid;
                }
            }
        }
    """

    expect = """
    RES: enter_actor
    RES: enter_traverse
    RES: enter_traverse
    RES: leaf
    RES: enter_traverse
    RES: leave_traverse
    RES: leaf
    RES: enter_traverse
    RES: leave_traverse
    RES: leave_traverse
    RES: leave_traverse
    RES: leave_actor
    """
    run_unit_test(
        dirconfig, 
        content, 
        expect,
        "top_activity_listener.sv")