import os
import pytest
from .simple_test_flow import run_unit_test
from .sim_util import sim_dvflow as dvflow

def test_simple_reg(dvflow):
    content = """
        import std_pkg::*;
        import addr_reg_pkg::*;
        import executor_pkg::*;
        pure component simple_regs : reg_group_c {
            reg_c<bit[32]>        R1;
            reg_c<bit[32]>        R2;

            function bit[64] get_offset_of_instance(string name) {
                if (name == "R1") {
                    return 0;
                } else if (name == "R2") {
                    return 4;
                } else {
                    return 0xFFFF_FFFF_FFFF_FFFF;
                }
            }
            function bit[64] get_offset_of_instance_array(string name, int index) {
                return 0;
            }
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
            simple_regs                     regs;
            transparent_addr_space_c<>      aspace;
            executor_group_default_c        exec_group;   
            my_executor                     executor;

            exec init_down {
                addr_handle_t reg_addr;
                transparent_addr_region_s<> region;

                exec_group.add_executor(executor);
                print("RES: init_down\\n");

                region.addr = 0x80000000;
                region.size = 0x00010000;
                reg_addr = aspace.add_nonallocatable_region(region);
                regs.set_handle(reg_addr);
            }
            action Entry {
                exec body {
                    comp.regs.R1.write_val(1);
                    comp.regs.R2.write_val(2);
                }
            }
        }
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

    expect = """
    RES: init_down
    RES: write32 0x80000000 0x00000001
    RES: write32 0x80000004 0x00000002
    """
    run_unit_test(
        dvflow, 
        content, 
        expect,
        test_top="top_custom_api_inc.sv",
        extra_content={
            "custom_api.sv": custom_api
        },
        debug=False)

@pytest.mark.skip(reason="Not implemented")
def test_reg_get_handle(dvflow):
    content = """
        import std_pkg::*;
        import addr_reg_pkg::*;
        import executor_pkg::*;
        pure component simple_regs : reg_group_c {
            reg_c<bit[32]>        R1;
            reg_c<bit[32]>        R2;

            function bit[64] get_offset_of_instance(string name) {
                if (name == "R1") {
                    return 0;
                } else if (name == "R2") {
                    return 4;
                } else {
                    return 0xFFFF_FFFF_FFFF_FFFF;
                }
            }
            function bit[64] get_offset_of_instance_array(string name, int index) {
                return 0;
            }
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
            simple_regs                     regs;
            transparent_addr_space_c<>      aspace;
            executor_group_default_c        exec_group;   
            my_executor                     executor;

            exec init_down {
                addr_handle_t reg_addr;
                transparent_addr_region_s<> region;
                print("RES: init_down\\n");

                region.addr = 0x80000000;
                region.size = 0x00010000;
                reg_addr = aspace.add_nonallocatable_region(region);
                regs.set_handle(reg_addr);

                exec_group.add_executor(executor);
            }
            action Entry {
                exec body {
                    addr_handle_t reg_addr = comp.regs.R1.get_handle();

                    write32(reg_addr, 20);
                }
            }
        }
    """
    expect = """
    RES: init_down
    RES: write32 0x80000000 0x00000001
    RES: write32 0x80000104 0x00000002
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
        debug=False)

def test_group_array(dvflow):
    content = """
        import std_pkg::*;
        import addr_reg_pkg::*;
        import executor_pkg::*;

        package pkg {
        struct MyS { 
        int a;
        }
        }
        pure component subgroup : reg_group_c {
            reg_c<bit[32]>        R1;
            reg_c<bit[32]>        R2;

            function bit[64] get_offset_of_instance(string name) {
                if (name == "R1") {
                    return 0;
                } else if (name == "R2") {
                    return 4;
                } else {
                    return 0xFFFF_FFFF_FFFF_FFFF;
                }
            }
            function bit[64] get_offset_of_instance_array(string name, int index) {
                return 0;
            }
        }
        pure component simple_regs : reg_group_c {
            subgroup              sub[2];

            function bit[64] get_offset_of_instance(string name) {
                if (name == "sub") {
                    return 0x100;
                }
            }

            function bit[64] get_offset_of_instance_array(string name, int index) {
                if (name == "sub") {
                    return (0x100 * index);
                }
                return 0;
            }
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
            simple_regs                     regs;
            transparent_addr_space_c<>      aspace;
            executor_group_default_c        exec_group;   
            my_executor                     executor;

            exec init_down {
                addr_handle_t reg_addr;
                transparent_addr_region_s<> region;
                print("RES: init_down\\n");

                region.addr = 0x80000000;
                region.size = 0x00010000;
                reg_addr = aspace.add_nonallocatable_region(region);
                regs.set_handle(reg_addr);

                exec_group.add_executor(executor);
            }
            action Entry {
                exec body {
                    comp.regs.sub[0].R1.write_val(1);
                    comp.regs.sub[1].R2.write_val(2);
                }
            }
        }
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

    expect = """
    RES: init_down
    RES: write32 0x80000000 0x00000001
    RES: write32 0x80000104 0x00000002
    """
    run_unit_test(
        dvflow, 
        content, 
        expect,
        test_top="top_custom_api_inc.sv",
        extra_content={
            "custom_api.sv": custom_api
        },
        debug=False)

def test_group_array_action_field_index(dvflow):
    content = """
        import std_pkg::*;
        import addr_reg_pkg::*;
        import executor_pkg::*;

        package pkg {
        struct MyS { 
        int a;
        }
        }
        pure component subgroup : reg_group_c {
            reg_c<bit[32]>        R1;
            reg_c<bit[32]>        R2;

            function bit[64] get_offset_of_instance(string name) {
                if (name == "R1") {
                    return 0;
                } else if (name == "R2") {
                    return 4;
                } else {
                    return 0xFFFF_FFFF_FFFF_FFFF;
                }
            }
            function bit[64] get_offset_of_instance_array(string name, int index) {
                return 0;
            }
        }
        pure component simple_regs : reg_group_c {
            subgroup              sub[2];

            function bit[64] get_offset_of_instance(string name) {
                if (name == "sub") {
                    return 0x100;
                }
            }

            function bit[64] get_offset_of_instance_array(string name, int index) {
                if (name == "sub") {
                    return (0x100 * index);
                }
                return 0;
            }
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
            simple_regs                     regs;
            transparent_addr_space_c<>      aspace;
            executor_group_default_c        exec_group;   
            my_executor                     executor;

            exec init_down {
                addr_handle_t reg_addr;
                transparent_addr_region_s<> region;
                print("RES: init_down\\n");

                region.addr = 0x80000000;
                region.size = 0x00010000;
                reg_addr = aspace.add_nonallocatable_region(region);
                regs.set_handle(reg_addr);

                exec_group.add_executor(executor);
            }
            action Entry {
                int idx = 0;
                exec body {
                    comp.regs.sub[idx+0].R1.write_val(1);
                    comp.regs.sub[idx+1].R2.write_val(2);
                }
            }
        }
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

    expect = """
    RES: init_down
    RES: write32 0x80000000 0x00000001
    RES: write32 0x80000104 0x00000002
    """
    run_unit_test(
        dvflow, 
        content, 
        expect,
        test_top="top_custom_api_inc.sv",
        extra_content={
            "custom_api.sv": custom_api
        },
        debug=False)
