

FIND_PACKAGE( Boost )

set(Boost_USE_STATIC_LIBS OFF)

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

message(STATUS ${Boost_INCLUDE_DIR})

include_directories(include)

add_library(controle-prog src/controle-prog.cpp)

add_executable(server main.cpp)

target_link_libraries(server LINK_PUBLIC controle-prog )

