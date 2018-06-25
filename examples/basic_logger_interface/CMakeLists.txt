cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-unknown-pragmas -Werror")

project(basic_logger_interface)
include_directories("${CMAKE_CURRENT_LIST_DIR}/../../include")
add_executable(${PROJECT_NAME} main.cpp)
