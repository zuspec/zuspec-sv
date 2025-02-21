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
                    print("RES: Hello World\\n");
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
        import executor_pkg::*;
        pure component simple_regs : reg_group_c {
            reg_c<bit[32]>        R1;
            reg_c<bit[32]>        R2;
        }

        /*
        import target function void write32(addr_handle_t addr, bit[32] data);
        component mem_executor_c : executor_c<> {
            function void write32(addr_handle_t addr, bit[32] data) {
                ::write32(addr, data);
            }
        }
         */

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
    Fatal: executor write32 not implemented
    """
    run_unit_test(
        dirconfig, 
        content, 
        expect,
        "top_default_api.sv",
        "Fatal:",
        debug=True)

def test_import_func_void(dirconfig):
    content = """
        import std_pkg::*;
        import addr_reg_pkg::*;

        import target function void my_write(bit[32] addr, bit[32] data);

        component pss_top {
            action Entry {
                exec body {
                    my_write(0, 4);
                }
            }
        }
    """
    custom_api = """
        class custom_api extends pss_import_api;
            virtual task my_write(int unsigned addr, int unsigned data);
                $display("RES: my_write(%0d, %0d)", addr, data);
            endtask
        endclass
"""
    
    expect = """
    RES: my_write(0, 4)
    """
    run_unit_test(
        dirconfig, 
        content, 
        expect,
        "top_custom_api_inc.sv",
        "RES:",
        debug=False,
        extra_content={
            "custom_api.svh": custom_api })
    
def test_import_func_retval(dirconfig):
    content = """
        import std_pkg::*;
        import addr_reg_pkg::*;

        import target function bit[32] my_read(bit[32] addr);

        component pss_top {
            action Entry {
                exec body {
                    bit[32] data;
                    data = my_read(0);
                    message(LOW, "RES: data = %d", data);
                }
            }
        }
    """
    custom_api = """
        class custom_api extends pss_import_api;
            virtual task my_read(output int unsigned __retval, input int unsigned addr);
                $display("RES: my_read(%0d)", addr);
                __retval = 42;
            endtask
        endclass
"""
    
    expect = """
    RES: my_read(0)
    RES: data = 42
    """
    run_unit_test(
        dirconfig, 
        content, 
        expect,
        "top_custom_api_inc.sv",
        "RES:",
        debug=False,
        extra_content={
            "custom_api.svh": custom_api })