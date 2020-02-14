cmake_minimum_required(VERSION 2.8)

LIST(APPEND CMAKE_MODULE_PATH
	"${CMAKE_CURRENT_SOURCE_DIR}/CMake/modules")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SANITIZE_CXX_FLAGS "")
#set(SANITIZE_CXX_FLAGS "-fsanitize=address -g -O1 -fno-omit-frame-pointer")
#set(SANITIZE_CXX_FLAGS "-fsanitize=memory -g -O1 -fno-omit-frame-pointer -fno-optimize-sibling-calls")
#set(SANITIZE_CXX_FLAGS "-fsanitize=undefined")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SANITIZE_CXX_FLAGS} -pedantic -Werror")

project(hfsm2_test)

include_directories("${CMAKE_CURRENT_LIST_DIR}/external"
					"${CMAKE_CURRENT_LIST_DIR}/include")

file(GLOB SOURCE_FILES "test/*.cpp" "test/shared/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_test(NAME hfsm2_test COMMAND hfsm2_test)

add_custom_command(TARGET hfsm2_test
				   POST_BUILD
				   COMMAND hfsm2_test)

if ("x_${CMAKE_BUILD_TYPE}" STREQUAL "x_Coverage")
	set (TEST_PROJECT hfsm2_test)
	include (coverage)
endif ("x_${CMAKE_BUILD_TYPE}" STREQUAL "x_Coverage")
