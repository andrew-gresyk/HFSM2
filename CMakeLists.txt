cmake_minimum_required (VERSION 3.1)
project (HFSM CXX)

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_EXTENSIONS Off)

file (GLOB_RECURSE SRCS test/*.cpp)
file (GLOB_RECURSE HDRS hfsm/*.hpp)
file (GLOB_RECURSE INLS hfsm/*.inl)

add_executable (hfsm_test ${SRCS} ${HDRS} ${INLS})
target_include_directories (hfsm_test PRIVATE hfsm)
