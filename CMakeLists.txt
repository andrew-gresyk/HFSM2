cmake_minimum_required(VERSION 3.5)

LIST(APPEND CMAKE_MODULE_PATH
	"${CMAKE_CURRENT_SOURCE_DIR}/CMake/modules")

set(SANITIZE_CXX_FLAGS "")
#set(SANITIZE_CXX_FLAGS "-fsanitize=address -g -O1 -fno-omit-frame-pointer")
#set(SANITIZE_CXX_FLAGS "-fsanitize=memory -g -O1 -fno-omit-frame-pointer -fno-optimize-sibling-calls")
#set(SANITIZE_CXX_FLAGS "-fsanitize=undefined")

project(hfsm2_test)

file(GLOB SOURCE_FILES "test/*.cpp" "test/shared/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
	"${CMAKE_CURRENT_LIST_DIR}/external"
	"${CMAKE_CURRENT_LIST_DIR}/include")

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_11)

if(MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -pedantic)
endif()

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})

add_custom_command(TARGET ${PROJECT_NAME}
				   POST_BUILD
				   COMMAND ${PROJECT_NAME})

if ("x_${CMAKE_BUILD_TYPE}" STREQUAL "x_Coverage")
	set (TEST_PROJECT ${PROJECT_NAME})
	include (coverage)
endif ("x_${CMAKE_BUILD_TYPE}" STREQUAL "x_Coverage")
