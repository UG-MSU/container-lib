project(.)

cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
add_executable(library main.cpp)

add_library(. src/container-lib.cpp)
target_link_libraries(library .)