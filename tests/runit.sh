#!/bin/bash

script_dir=$(dirname $(realpath $0))
zuspec_sv=$(realpath $script_dir/..)

#export PATH=/project/tools/vivado/Vivado/2021.2/bin:$PATH
export PATH=/project/tools/vivado/Vivado/2023.1/bin:$PATH
export PYTHONPATH=$zuspec_sv/python:$PYTHONPATH
export LD_LIBRARY_PATH=`$zuspec_sv/packages/python/bin/python -m zsp_sv ldpath`:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$zuspec_sv/build/src:$LD_LIBRARY_PATH

$zuspec_sv/packages/python/bin/python -m zuspec generate-sv test.pss

xvlog -sv \
	$zuspec_sv/src/include/zsp/sv/zuspec.sv \
	pss_api_pkg.sv \
	top.sv
if test $? -ne 0; then exit 1; fi

ls $zuspec_sv/build/src
xelab top --sv_root $zuspec_sv/build/src --sv_lib libzsp-sv.so
if test $? -ne 0; then exit 1; fi

xsim top --runall --testplusarg zuspec.pssfiles=test.pss --testplusarg zuspec.load=1 --testplusarg zuspec.debug=0
#xsim top --runall 

echo $zuspec_sv
