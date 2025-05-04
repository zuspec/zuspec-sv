import os
import pytest
from .simple_test_flow import run_unit_test
from .sim_util import sim_dvflow as dvflow

def test_simple_memwrite(dvflow):
    content = """
        import std_pkg::*;
        import addr_reg_pkg::*;
        import executor_pkg::*;
        pure component simple_regs : reg_group_c {
            reg_c<bit[32]>        R1;
            reg_c<bit[32]>        R2;
        }
        import target function void my_write32(bit[64] addr, bit[32] data);
        import target function bit[32] my_read32(bit[64] addr);

        component my_executor : executor_c<> {
            target function void write32(addr_handle_t hndl, bit[32] data) {
                my_write32(addr_value(hndl), data);
            }
            target function bit[32] read32(addr_handle_t hndl) {
                return my_read32(addr_value(hndl));
            }
        }

        component foo { }

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

                set_executor(executor);
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
    package custom_api_pkg;
    import pss_types::*;
    class custom_api extends pss_import_api;
        virtual task my_write32(longint unsigned addr, int unsigned data);
            $display("RES: write32 0x%08h 0x%08h", addr, data);
        endtask
    endclass
    endpackage
    """
    run_unit_test(
        dvflow, 
        content, 
        expect,
        test_top="top_custom_api_inc.sv",
        extra_content={
            "custom_api.svh": custom_api
        },
        debug=True)

def test_simple_memread(dvflow):
    content = """
        import std_pkg::*;
        import addr_reg_pkg::*;
        import executor_pkg::*;

        pure component simple_regs : reg_group_c {
            reg_c<bit[32]>        R1;
            reg_c<bit[32]>        R2;
        }

        import target function void my_write32(bit[64] addr, bit[32] data);
        import target function bit[32] my_read32(bit[64] addr);

        component my_executor : executor_c<> {
            target function void write32(addr_handle_t hndl, bit[32] data) {
                my_write32(addr_value(hndl), data);
            }
            target function bit[32] read32(addr_handle_t hndl) {
                return my_read32(addr_value(hndl));
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

                set_executor(executor);
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

    custom_api = """
    package custom_api_pkg;
    import pss_types::*;
    class custom_api extends pss_import_api;
        virtual task my_write32(longint unsigned addr, int unsigned data);
            $display("RES: write32 0x%08h 0x%08h", addr, data);
        endtask
        virtual task my_read32(output int unsigned __retval, input longint unsigned addr);
            __retval = 32'h20;
            $display("RES: read32 0x%08h 0x%08h", addr, __retval);
        endtask
    endclass
    endpackage
    """
    run_unit_test(
        dvflow, 
        content, 
        expect,
        test_top="top_custom_api_inc.sv",
        extra_content={
            "custom_api.svh": custom_api
        },
        debug=False)
