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
        component pss_top {
            simple_regs                     regs;
            transparent_addr_space_c<>      aspace;

            exec init_down {
                addr_handle_t reg_addr;
                transparent_addr_region_s<> region;
                print("RES: init_down");

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
    expect = """
    RES: init_down
    RES: write32 0x80000000 0x00000001
    RES: write32 0x80000004 0x00000002
    """
    run_unit_test(dirconfig, content, expect)

def test_reg_get_handle(dirconfig):
    content = """
        import std_pkg::*;
        import addr_reg_pkg::*;
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
        component pss_top {
            simple_regs                     regs;
            transparent_addr_space_c<>      aspace;

            exec init_down {
                addr_handle_t reg_addr;
                transparent_addr_region_s<> region;
                print("RES: init_down");

                region.addr = 0x80000000;
                region.size = 0x00010000;
                reg_addr = aspace.add_nonallocatable_region(region);
                regs.set_handle(reg_addr);
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
    RES: init_up
    """
    run_unit_test(dirconfig, content, expect)

def test_group_array(dirconfig):
    content = """
        import std_pkg::*;
        import addr_reg_pkg::*;
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
        component pss_top {
            simple_regs                     regs;
            transparent_addr_space_c<>      aspace;

            exec init_down {
                addr_handle_t reg_addr;
                transparent_addr_region_s<> region;
                print("RES: init_down");

                region.addr = 0x80000000;
                region.size = 0x00010000;
                reg_addr = aspace.add_nonallocatable_region(region);
                regs.set_handle(reg_addr);
            }
            action Entry {
                exec body {
                    comp.regs.sub[0].R1.write_val(1);
                    comp.regs.sub[1].R2.write_val(2);
                }
            }
        }
    """
    expect = """
    RES: init_down
    RES: write32 0x80000000 0x00000001
    RES: write32 0x80000104 0x00000002
    """
    run_unit_test(dirconfig, content, expect)