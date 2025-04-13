import os
import pytest
import sys
from .simple_test_flow import run_unit_test
from .sim_util import sim_dvflow as dvflow

def test_smoke(dvflow):
    content = """
        import std_pkg::*;

        buffer B1 { }

        component pss_top {
            action Prod {
                output B1 dat_o;
            }
            action Cons {
                input B1 dat_i;
            }
            action Entry {
                Prod prod;
                Cons cons;
                activity {
                    prod;
                    cons;
                    bind prod.dat_o cons.dat_i;
                }

                exec post_solve {
                    print("RES: Hello World!\\n");
                }
            }
        }
    """

    expect = """
    RES: Hello World!
    """
    run_unit_test(dvflow, content, expect, debug=True)
