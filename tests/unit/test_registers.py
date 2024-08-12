import os
import pytest
import pytest_fv as pfv
from pytest_fv.fixtures import *
import sys
from .simple_test_flow import run_unit_test

def test_simple_reg(dirconfig):
    content = """
        import std_pkg::*;
        import addr_reg_pkg::*;
        pure component simple_regs : reg_group_c {
            reg_c<bit[32]>        R1;
            reg_c<bit[32]>        R2;
        }
        component pss_top {
            simple_regs     regs;
            exec init_down {
                print("RES: init_down");
            }
            action Entry {
                exec body {
                    comp.regs.R1.write_val(1);
                    comp.regs.R2.write_val(2);
                }
            }
        }
    """
    expect = """
    RES: init_down
    RES: init_up
    """
    run_unit_test(dirconfig, content, expect)
