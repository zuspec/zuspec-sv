import os
import pytest
from .simple_test_flow import run_unit_test
from .sim_util import sim_dvflow as dvflow

def test_executor_setup(dvflow):
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
    run_unit_test(dvflow, content, expect)