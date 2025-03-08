import os
import pytest
import pytest_fv as pfv
from pytest_fv.fixtures import *
import sys
from .simple_test_flow import run_unit_test

def test_executor_setup(dirconfig):
    content = """
        import std_pkg::*;
        import executor_pkg::*;

        component pss_top {
            executor_c <> default_executor;
            action Entry {
                exec post_solve {
                    print("RES: Hello\\n");
                }
            }
        }
    """
    expect = """
    RES: Hello
    """
    run_unit_test(dirconfig, content, expect)