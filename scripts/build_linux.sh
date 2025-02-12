#!/bin/sh -x

yum update -y
yum install -y glibc-static
yum install -y java-11-openjdk-devel uuid-devel libuuid-devel

echo "BUILD_NUM=${BUILD_NUM}" >> python/zsp_sv/__build_num__.py
${IVPM_PYTHON} -m pip install --pre ivpm cython setuptools
${IVPM_PYTHON} -m ivpm update -a --py-prerls-packages

echo "Requirements Files"
cat packages/*.txt

PYTHON=./packages/python/bin/python
${PYTHON} -m pip install auditwheel ninja wheel cython
# Workaround for pypa bug
${PYTHON} -m pip install -U twine packaging
${PYTHON} setup.py bdist_wheel

for whl in dist/*.whl; do
    ${PYTHON} -m auditwheel repair --only-plat $whl
    rm $whl
done
