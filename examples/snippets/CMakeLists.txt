cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

project(snippets)

include_directories("${CMAKE_CURRENT_LIST_DIR}/../../external"
					"${CMAKE_CURRENT_LIST_DIR}/../../include")

file(GLOB SOURCE_FILES "*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_test(NAME snippets COMMAND snippets)

add_custom_command(TARGET snippets
				   POST_BUILD
				   COMMAND snippets)
