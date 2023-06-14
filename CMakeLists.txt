cmake_minimum_required(VERSION 3.22)
FIND_PACKAGE( Boost )
project(container-lib)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -pg")
set(Boost_USE_STATIC_LIBS OFF)
include_directories(include)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
message(STATUS ${Boost_INCLUDE_DIR})
# add_library(container-lib src/container-lib.cpp src/cgroups.c)
add_library(container-lib src/container-lib.cpp src/cgroups.cpp src/exception.cpp src/controle-prog.cpp src/interpreted-test.cpp src/linking-server.cpp)


add_executable(test_exec main.cpp)
add_executable(server server.cpp)
add_custom_target(
   DockerClient
   COMMAND make
   WORKING_DIRECTORY extern/DockerClient
)
target_link_libraries(test_exec container-lib)
target_link_libraries(server LINK_PUBLIC controle-prog )
add_dependencies(server DockerClient)
add_subdirectory(extern/pybind11)
pybind11_add_module(container_lib_py src/wrapper.cpp src/container-lib.cpp src/cgroups.cpp src/exception.cpp)
