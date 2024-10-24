
import os
import pytest
import pytest_fv as pfv
from pytest_fv.fixtures import *
import sys
from .simple_test_flow import run_unit_test

def disabled_test_smoke(dirconfig):
    content = """
        import std_pkg::*;
        import addr_reg_pkg::*;

        component pss_top {
            transparent_addr_space_c<>      aspace;

            exec init_down {
                addr_handle_t reg_addr;
                transparent_addr_region_s<> region;
                print("RES: init_down");

                region.addr = 0x80000000;
                region.size = 0x00010000;
                reg_addr = aspace.add_nonallocatable_region(region);
            }

            action Entry {
                exec body {
                }
            }
        }
    """
    expect = """
    RES: init_down
    RES: write32 0x80000000 0x00000001
    RES: write32 0x80000004 0x00000002
    """
    run_unit_test(dirconfig, content, expect)
