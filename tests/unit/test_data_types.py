import os
import pytest
import sys
from .simple_test_flow import run_unit_test


def disabled_test_int_array(dirconfig):
    content = """
        import std_pkg::*;
        component pss_top {
            action Entry {
                exec post_solve {
                    int my_arr[10];
                    my_arr[0] = 1;
                    my_arr[1] = 2;
                    print("RES: Hello %d", my_arr[0]);
                    print("RES: Hello %d", my_arr[1]);
                }
            }
        }
    """
    expect = """
    RES: Hello
    RES: Hello
    """
    run_unit_test(dirconfig, content, expect)