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

