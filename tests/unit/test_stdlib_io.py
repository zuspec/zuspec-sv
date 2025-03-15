
import os
import pytest
from .simple_test_flow import run_unit_test
from .sim_util import sim_dvflow as dvflow

def test_message_basics(dvflow):
    content = """
        import std_pkg::*;
        component pss_top {

            action Entry {
                exec body {
                    message(LOW, "RES: Hello World!");
                }
            }
        }
    """
    expect = """
    RES: Hello World!
    """
    run_unit_test(dvflow, content, expect, debug=False)

def test_message_verbosity_filter(dvflow):
    content = """
        import std_pkg::*;
        component pss_top {

            action Entry {
                exec body {
                    message(3, "RES: Should be skipped");
                    message(0, "RES: Hello World!");
                }
            }
        }
    """
    expect = """
    RES: Hello World!
    """
    run_unit_test(dvflow, content, expect)