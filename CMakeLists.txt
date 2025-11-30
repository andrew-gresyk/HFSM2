cmake_minimum_required(VERSION 3.10)

project(hfsm2 VERSION 2.8.0 LANGUAGES CXX)

# Create an interface library (header-only)
add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# Set include directories for users of this library
target_include_directories(${PROJECT_NAME}
		INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
)

# Set C++11 requirement for users of this library
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_11)

# Install the library
install(TARGETS ${PROJECT_NAME}
		EXPORT ${PROJECT_NAME}Targets
		INCLUDES DESTINATION include
)

# Install the headers
install(DIRECTORY include/ DESTINATION include)

# Export the targets
export(EXPORT ${PROJECT_NAME}Targets
		FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake"
		NAMESPACE ${PROJECT_NAME}::
)

# Create and install package configuration files
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
		"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
		VERSION ${PROJECT_VERSION}
		COMPATIBILITY SameMajorVersion
)

# Simple config file approach (no template required)
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
		"include(CMakeFindDependencyMacro)
include(\"\${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}Targets.cmake\")
")

# Install the configuration files
install(FILES
		"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
		"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
		DESTINATION lib/cmake/${PROJECT_NAME}
)

install(EXPORT ${PROJECT_NAME}Targets
		FILE ${PROJECT_NAME}Targets.cmake
		NAMESPACE ${PROJECT_NAME}::
		DESTINATION lib/cmake/${PROJECT_NAME}
)

# Testing section
option(HFSM2_BUILD_TESTS "Build the HFSM2 tests" OFF)
if(HFSM2_BUILD_TESTS)
	enable_testing()

	file(GLOB TEST_SOURCE_FILES
		 "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp"
		 "${CMAKE_CURRENT_SOURCE_DIR}/test/*/*.cpp")
	add_executable(${PROJECT_NAME}_test ${TEST_SOURCE_FILES})

	target_include_directories(${PROJECT_NAME}_test PRIVATE
			"${CMAKE_CURRENT_SOURCE_DIR}/external"
			"${CMAKE_CURRENT_SOURCE_DIR}/include")

	target_compile_features(${PROJECT_NAME}_test PRIVATE cxx_std_11)

	if(MSVC)
		target_compile_options(${PROJECT_NAME}_test PRIVATE /W4 /WX)
	else()
		target_compile_options(${PROJECT_NAME}_test PRIVATE -Werror -Wall -Wextra -Wpedantic -Wshadow -Wold-style-cast)
	endif()

	add_test(NAME ${PROJECT_NAME}_test COMMAND ${PROJECT_NAME}_test)

	add_custom_command(TARGET ${PROJECT_NAME}_test
			POST_BUILD
			COMMAND ${PROJECT_NAME}_test)
endif()

# Examples section
option(HFSM2_BUILD_EXAMPLES "Build the examples" OFF)
if(HFSM2_BUILD_EXAMPLES)
	# Check if examples directory exists
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples")
		# Get all subdirectories in examples folder
		file(GLOB EXAMPLE_DIRS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/examples" "${CMAKE_CURRENT_SOURCE_DIR}/examples/*")

		foreach(EXAMPLE_DIR ${EXAMPLE_DIRS})
			# Check if the subdirectory contains a CMakeLists.txt
			if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples/${EXAMPLE_DIR}/CMakeLists.txt")
				message(STATUS "Adding example: ${EXAMPLE_DIR}")
				add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/examples/${EXAMPLE_DIR}")
			endif()
		endforeach()
	else()
		message(STATUS "Examples directory not found, skipping examples")
	endif()
endif()