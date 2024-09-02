import os
import pytest
import pytest_fv as pfv
from pytest_fv.fixtures import *
import sys
from .simple_test_flow import run_unit_test

def test_subactivity(dirconfig):
    content = """
        import std_pkg::*;
        component pss_top {

            action Leaf {
                exec post_solve {
                    print("RES: leaf");
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
                    print("RES: leaf %d", val);
                }
            }

            action Mid {
                int i;
                activity {
                    do Leaf;
                    do Leaf with {
                      val == 2;
                    }
                    do Leaf;
//                    do Leaf with {
//                      val == 1;
//                    };
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
    run_unit_test(dirconfig, content, expect)
