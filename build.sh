#!/bin/bash

dir=$(cd $(dirname $0); pwd)
cd $dir && mkdir -p build && cmake -S . -B ./build && cd build && make