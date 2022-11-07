set(CMAKE_MINIMUM_REQUIRED_VERSION 3.25.0)

project(forking-task CXX)
set(CMAKE_CXX_STANDARD 17)


add_executable(forking-task container_lib/src/cont_lib.cpp)