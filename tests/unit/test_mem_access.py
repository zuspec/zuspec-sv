import os
import pytest
import pytest_fv as pfv
from pytest_fv.fixtures import *
import sys
from .simple_test_flow import run_unit_test

@pytest.mark.skip(reason="Not implemented")
def test_simple_memwrite(dirconfig):
    content = """
        import std_pkg::*;
        import addr_reg_pkg::*;
        import executor_pkg::*;
        pure component simple_regs : reg_group_c {
            reg_c<bit[32]>        R1;
            reg_c<bit[32]>        R2;
        }
        import target function void my_write32(bit[64] addr, bit[32] data);

        component my_executor : executor_c<> {
            function void write32(addr_handle_t addr, bit[32] data) {
                my_write32(addr_value(addr), data);
            }
        }

        component pss_top {
            transparent_addr_space_c<>      aspace;
            addr_handle_t                   hndl;
            my_executor                     executor;

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
                    write32(comp.hndl, 64);
                }
            }
        }
    """
    expect = """
    RES: write32 0x80000000 0x00000020
    RES: write32 0x80000000 0x00000040
    """
    
    custom_api = """
    class custom_api extends pss_import_api;
        virtual task my_write32(longint unsigned addr, int unsigned data);
            $display("RES: write32 0x%08h 0x%08h", addr, data);
        endtask
    endclass
    """
    run_unit_test(
        dirconfig, 
        content, 
        expect,
        test_top="top_custom_api_inc.sv",
        extra_content={
            "custom_api.svh": custom_api
        })

@pytest.mark.skip(reason="Not implemented")
def test_simple_memread(dirconfig):
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
                    bit[32] val;
                    write32(comp.hndl, 32);
                    val = read32(comp.hndl);
                }
            }
        }
    """
    expect = """
    RES: write32 0x80000000 0x00000020
    RES: read32 0x80000000 0x00000020
    """
    run_unit_test(dirconfig, content, expect,
                  debug=False)
