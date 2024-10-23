#!/bin/bash

proj_dir=$(dirname $(dirname $(realpath $0)))

./packages/python/bin/python3 -m pytest ${proj_dir}/tests/test_exec_stmts.py
