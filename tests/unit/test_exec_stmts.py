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
                        print("RES: Hello");
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

def test_repeat_count(dirconfig):
    content = """
        import std_pkg::*;
        component pss_top {
            action Entry {
                exec post_solve {
                    repeat (2) {
                        print("RES: Hello");
                    }
                }
            }
        }
    """
    expect = """
    RES: Hello
    RES: Hello
    """
    run_unit_test(dirconfig, content, expect, debug=True)

def test_repeat_count_var(dirconfig):
    content = """
        import std_pkg::*;
        component pss_top {
            action Entry {
                exec post_solve {
                    repeat (i : 2) {
                        print("RES: Hello %d", i);
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

def test_while(dirconfig):
    content = """
        import std_pkg::*;
        component pss_top {
            action Entry {
                exec post_solve {
                    int i = 0;
                    while (i < 2) {
                        print("RES: Hello %d", i);
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
                        print("RES: Hello %d", i);
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
                        print("RES: i==1");
                    } else {
                        print("RES: !(i==1)");
                    }
                    if (i != 1) {
                        print("RES: i!=1");
                    } else {
                        print("RES: !(i!=1)");
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
                        print("RES: i==2");
                    } else if (i == 1) {
                        print("RES: i==1");
                    } else {
                        print("RES: !(i==1)");
                    }
                    if (i != 1) {
                        print("RES: i!=1");
                    } else if (i != 2) {
                        print("RES: i!=1");
                    } else {
                        print("RES: !(i!=1)");
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

def test_assign(dirconfig):
    content = """
        import std_pkg::*;
        component pss_top {
            action Entry {
                exec post_solve {
                    int i = 1;
                    print("RES: i==1 ; i=%d", i);
                    i += 1;
                    print("RES: i==2 ; i=%d", i);
                    i = 1;
                    print("RES: i==1 ; i=%d", i);
                    i -= 1;
                    print("RES: i==0 ; i=%d", i);
                    i |= 1;
                    print("RES: i==1 ; i=%d", i);
                    i |= 2;
                    print("RES: i==3 ; i=%d", i);
                    i &= 2;
                    print("RES: i==2 ; i=%d", i);
                    i <<= 2;
                    print("RES: i==8 ; i=%d", i);
                    i >>= 2;
                    print("RES: i==2 ; i=%d", i);
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