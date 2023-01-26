#!/bin/bash
#
#

BASE=$(dirname $BASH_SOURCE)
cwd=$(pwd)

cmake -S $BASE -B $BASE/build && \
    cmake --build $BASE/build && \
    cd build && \
    ctest --output-on-failure ;\
    cd $cwd
