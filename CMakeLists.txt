cmake_minimum_required(VERSION 3.22)

project(container-lib)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g")

include_directories(include)
# add_library(container-lib src/container-lib.cpp src/cgroups.c)
add_library(container-lib src/container-lib.cpp src/cgroups.cpp)
add_executable(test_exec main.cpp)
target_link_libraries(test_exec container-lib)
add_subdirectory(extern/pybind11)
pybind11_add_module(container_lib_py src/wrapper.cpp src/container-lib.cpp src/cgroups.cpp)
