cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
project(hfsm2 VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)

option(HFSM_BUILD_TESTS "Enable/disable building test executable" ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)

set(extra_cxx_flags "-Wall -Werror")
set(CMAKE_CXX_FLAGS "${extra_cxx_flags} ${CMAKE_CXX_FLAGS}")
set(CMAKE_C_FLAGS "${extra_cxx_flags} ${CMAKE_C_FLAGS}")

#-------------------------------------------------------------------------------
# hfsm2 target
#-------------------------------------------------------------------------------
add_library(hfsm2 INTERFACE)
target_include_directories(
  hfsm2 INTERFACE
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

#-------------------------------------------------------------------------------
# Install & export targets
#-------------------------------------------------------------------------------
set(config_install_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
set(include_install_dir "${CMAKE_INSTALL_INCLUDEDIR}")
set(version_config "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${PROJECT_NAME}Config.cmake")
set(targets_export_name "${PROJECT_NAME}Targets")
set(namespace "${PROJECT_NAME}::")

# Generate package version file.
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${version_config}" COMPATIBILITY SameMajorVersion
)

# Export hfsm2 target.
install(
  TARGETS hfsm2
  EXPORT "${targets_export_name}"
  INCLUDES DESTINATION "${include_install_dir}"
)

# Install hfsm2 headers.
install(
  DIRECTORY "include/${PROJECT_NAME}"
  DESTINATION "${include_install_dir}"
)

# Install project version file.
install(
  FILES "${version_config}"
  DESTINATION "${config_install_dir}"
)

# Install project config file.
install(
  EXPORT "${targets_export_name}"
  NAMESPACE "${namespace}"
  DESTINATION "${config_install_dir}"
  FILE ${project_config}
)

# Export build directory config file.
export(
  EXPORT ${targets_export_name}
  NAMESPACE "${namespace}"
  FILE ${project_config}
)

# Register project in CMake user registry.
export(PACKAGE ${PROJECT_NAME})

if(HFSM_BUILD_TESTS)
  enable_testing()
  add_subdirectory(test)
endif(HFSM_BUILD_TESTS)
