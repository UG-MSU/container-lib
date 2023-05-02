cmake_minimum_required(VERSION 3.22)

project(interpreted-test-lib)
set(CMAKE_CXX_STANDARD 17)

include_directories(inlcude)

#creates docker-lib
#include_directories(include extern/DockerClient/include/ extern/DockerClient/rapidjson/include/) #этой строки быть не должно, костыль, чтобы собиралось
add_subdirectory(extern/DockerClient)
add_library(docker-lib src/docker-lib.cpp)
add_library(linking-server src/linking-server.cpp)

add_executable(lib main.cpp)

target_link_libraries(lib docker-lib linking-server)