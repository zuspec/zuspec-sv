import os
import pytest
import sys
from .simple_test_flow import run_unit_test

def disabled_test_smoke(dirconfig):
    content = """
        import std_pkg::*;
        buffer DatB {
            rand bit[16]    a, b;
        }
        component pss_top {
            pool DatB dat_b_p;
            bind dat_b_p *;

            action Prod {
                output DatB dat_o;
                exec post_solve {
                    print("RES: dat_o.a=%d", dat_o.a);
                }
            }

            action Cons {
                input DatB dat_i;
            }

            action Entry {
                Prod p;
                Cons c;
                activity {
                    p;
                    c;
                    bind p.dat_o c.dat_i;
                }
            }
        }
    """

    expect = """
    RES: a=1
    RES: b=2
    """
    run_unit_test(dirconfig, content, expect)
