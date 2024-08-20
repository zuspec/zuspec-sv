import os
import pytest
import pytest_fv as pfv
from pytest_fv.fixtures import *
import sys
from .simple_test_flow import run_unit_test

def test_repeat_count(dirconfig):
    content = """
        import std_pkg::*;
        component pss_top {
            action Entry {
                exec post_solve {
                    repeat (2) {
//                        print("RES: Hello");
                    }
                }
            }
        }
    """
    expect = """
    RES: Hello
    RES: Hello
    """
    run_unit_test(dirconfig, content, expect)
