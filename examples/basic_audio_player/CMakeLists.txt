cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

project(basic_audio_player)
include_directories("${CMAKE_CURRENT_LIST_DIR}/../../include")
add_executable(${PROJECT_NAME} main.cpp)
