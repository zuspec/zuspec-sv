import os
import pytest
import pytest_fv as pfv
from pytest_fv.fixtures import *
import sys
from .simple_test_flow import run_unit_test

def test_repeat_count(dirconfig):
    content = """
        import std_pkg::*;
        component pss_top {
            action Entry {
                exec post_solve {
                    repeat (2) {
                        print("RES: Hello\\n");
                    }
                }
            }
        }
    """
    expect = """
    RES: Hello
    RES: Hello
    """
    run_unit_test(dirconfig, content, expect)

# def test_repeat_count(dirconfig):
#     content = """
#         import std_pkg::*;
#         component pss_top {
#             action Entry {
#                 exec post_solve {
#                     repeat (2) {
#                         print("RES: Hello\\n");
#                     }
#                 }
#             }
#         }
#     """
#     expect = """
#     RES: Hello
#     RES: Hello
#     """
#     run_unit_test(dirconfig, content, expect, debug=True)

def test_repeat_count_var(dirconfig):
    content = """
        import std_pkg::*;
        component pss_top {
            action Entry {
                exec post_solve {
                    repeat (i : 2) {
                        print("RES: Hello %d\\n", i);
                    }
                }
            }
        }
    """
    expect = """
    RES: Hello 0
    RES: Hello 1
    """
    run_unit_test(dirconfig, content, expect, debug=True)

def test_repeat_local_var(dirconfig):
    content = """
        import std_pkg::*;
        component pss_top {
            action Entry {
                exec post_solve {
                    repeat (i : 2) {
                        int x = 1;
                        print("RES: Hello %d\\n", x);
                    }
                }
            }
        }
    """
    expect = """
    RES: Hello 1
    RES: Hello 1
    """
    run_unit_test(dirconfig, content, expect, debug=True)

def test_repeat_count_local_var(dirconfig):
    content = """
        import std_pkg::*;
        component pss_top {
            action Entry {
                exec post_solve {
                    repeat (i : 2) {
                        int x = i+1;
                        print("RES: Hello %d %d\\n", i, x);
                    }
                }
            }
        }
    """
    expect = """
    RES: Hello 0 1
    RES: Hello 1 2
    """
    run_unit_test(dirconfig, content, expect, debug=True)

def test_while(dirconfig):
    content = """
        import std_pkg::*;
        component pss_top {
            action Entry {
                exec post_solve {
                    int i = 0;
                    while (i < 2) {
                        print("RES: Hello %d\\n", i);
                        i += 1;
                    }
                }
            }
        }
    """
    expect = """
    RES: Hello 0
    RES: Hello 1
    """
    run_unit_test(dirconfig, content, expect, debug=True)

def test_repeat_while(dirconfig):
    content = """
        import std_pkg::*;
        component pss_top {
            action Entry {
                exec post_solve {
                    int i = 0;
                    repeat {
                        print("RES: Hello %d\\n", i);
                        i += 1;
                    } while (i < 2);
                }
            }
        }
    """
    expect = """
    RES: Hello 0
    RES: Hello 1
    """
    run_unit_test(dirconfig, content, expect, debug=True)

def test_if_else(dirconfig):
    content = """
        import std_pkg::*;
        component pss_top {
            action Entry {
                exec post_solve {
                    int i = 1;
                    if (i == 1) {
                        print("RES: i==1\\n");
                    } else {
                        print("RES: !(i==1)\\n");
                    }
                    if (i != 1) {
                        print("RES: i!=1\\n");
                    } else {
                        print("RES: !(i!=1)\\n");
                    }
                }
            }
        }
    """
    expect = """
    RES: i==1
    RES: !(i!=1)
    """
    run_unit_test(dirconfig, content, expect)

def test_if_elsif(dirconfig):
    content = """
        import std_pkg::*;
        component pss_top {
            action Entry {
                exec post_solve {
                    int i = 1;
                    if (i == 2) {
                        print("RES: i==2\\n");
                    } else if (i == 1) {
                        print("RES: i==1\\n");
                    } else {
                        print("RES: !(i==1)\\n");
                    }
                    if (i != 1) {
                        print("RES: i!=1\\n");
                    } else if (i != 2) {
                        print("RES: i!=1\\n");
                    } else {
                        print("RES: !(i!=1)\\n");
                    }
                }
            }
        }
    """
    expect = """
    RES: i==1
    RES: i!=1
    """
    run_unit_test(dirconfig, content, expect)

def test_exec_hier_ref(dirconfig):
    content = """
    import addr_reg_pkg::*;
    import std_pkg::*;

    component pss_top {
        exec init_down {
            transparent_addr_region_s<> region;
            
            region.addr = 5;

            print("RES: region.addr=%d\\n", region.addr);
        }

        action Entry {
        }
    }
    """
    expect = """
    RES: region.addr=5
    """
    run_unit_test(dirconfig, content, expect)

def test_assign(dirconfig):
    content = """
        import std_pkg::*;
        component pss_top {
            action Entry {
                exec post_solve {
                    int i = 1;
                    print("RES: i==1 ; i=%d\\n", i);
                    i += 1;
                    print("RES: i==2 ; i=%d\\n", i);
                    i = 1;
                    print("RES: i==1 ; i=%d\\n", i);
                    i -= 1;
                    print("RES: i==0 ; i=%d\\n", i);
                    i |= 1;
                    print("RES: i==1 ; i=%d\\n", i);
                    i |= 2;
                    print("RES: i==3 ; i=%d\\n", i);
                    i &= 2;
                    print("RES: i==2 ; i=%d\\n", i);
                    i <<= 2;
                    print("RES: i==8 ; i=%d\\n", i);
                    i >>= 2;
                    print("RES: i==2 ; i=%d\\n", i);
                }
            }
        }
    """
    expect = """
    RES: i==1 ; i=1
    RES: i==2 ; i=2
    RES: i==1 ; i=1
    RES: i==0 ; i=0
    RES: i==1 ; i=1
    RES: i==3 ; i=3
    RES: i==2 ; i=2
    RES: i==8 ; i=8
    RES: i==2 ; i=2
    """
    run_unit_test(dirconfig, content, expect)