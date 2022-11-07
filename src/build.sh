#!/bin/bash
c++ -O3 -Wall -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) wrapper.cpp  -o ../build/container_lib_py$(python3-config --extension-suffix) 