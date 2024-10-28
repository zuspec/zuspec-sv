import os
import pytest
import pytest_fv as pfv
from pytest_fv.fixtures import *
import sys
from .simple_test_flow import run_unit_test

def disabled_test_parse(dirconfig):
    with open(os.path.join(dirconfig.test_srcdir(), "data/tmp.pss"), "r") as fp:
        content = fp.read()

    expect = ""

    run_unit_test(dirconfig, content, expect)
