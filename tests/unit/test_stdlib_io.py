
import os
import pytest
import pytest_fv as pfv
from pytest_fv.fixtures import *
import sys
from .simple_test_flow import run_unit_test

def test_message_basics(dirconfig):
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
    run_unit_test(dirconfig, content, expect)

def test_message_verbosity_filter(dirconfig):
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
    run_unit_test(dirconfig, content, expect)