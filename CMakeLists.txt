cmake_minimum_required(VERSION 3.22)

project(container-lib)
set(CMAKE_CXX_STANDARD 17)

include_directories(include)
add_executable(library main.cpp)

add_library(container-lib src/container-lib.cpp)
target_link_libraries(library container-lib)
