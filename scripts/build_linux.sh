#!/bin/sh -x

yum update -y
yum install -y glibc-static
yum install -y java-11-openjdk-devel uuid-devel libuuid-devel

echo "BUILD_NUM=${BUILD_NUM}" >> python/zsp_sv/__build_num__.py
${IVPM_PYTHON} -m pip install ivpm cython
${IVPM_PYTHON} -m ivpm update -a

echo "Requirements Files"
cat packages/*.txt

PYTHON=./packages/python/bin/python
${PYTHON} -m pip install twine auditwheel ninja wheel cython
${PYTHON} setup.py bdist_wheel

for whl in dist/*.whl; do
    ${PYTHON} -m auditwheel repair $whl
    rm $whl
done
