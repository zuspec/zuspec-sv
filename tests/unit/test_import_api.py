import os
import pytest
import pytest_fv as pfv
from pytest_fv.fixtures import *
import sys
from .simple_test_flow import run_unit_test

def test_default_api_impl_no_access(dirconfig):
    content = """
        import std_pkg::*;
        component pss_top {

            action Entry {
                exec post_solve {
                    print("RES: Hello World");
                }
            }
        }
    """
    expect = """
    RES: Hello World
    """
    run_unit_test(
        dirconfig, 
        content, 
        expect,
        "top_default_api.sv")
    
def test_default_api_impl_mem_access(dirconfig):
    content = """
        import std_pkg::*;
        import addr_reg_pkg::*;
        pure component simple_regs : reg_group_c {
            reg_c<bit[32]>        R1;
            reg_c<bit[32]>        R2;
        }
        component pss_top {
            transparent_addr_space_c<>      aspace;
            addr_handle_t                   hndl;

            exec init_down {
                transparent_addr_region_s<> region;
//                addr_region_s<> region;
                region.addr = 0x80000000;
                region.size = 0x10000000;
                hndl = aspace.add_nonallocatable_region(region);
            }
            action Entry {
                exec body {
                    write32(comp.hndl, 32);
                }
            }
        }
    """
    expect = """
    Fatal: write32 not implemented
    """
    run_unit_test(
        dirconfig, 
        content, 
        expect,
        "top_default_api.sv",
        "Fatal:")
