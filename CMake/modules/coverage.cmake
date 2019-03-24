if (BUILD_COVERAGE)
  set(CMAKE_BUILD_TYPE "Debug")
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    find_program(GCOV_PATH gcov)
    find_program(GENHTML_PATH genhtml)
    find_program(LCOV_PATH lcov)

    if(NOT LCOV_PATH)
        message(FATAL_ERROR "lcov not found.")
    endif(NOT LCOV_PATH)

    if(NOT GCOV_PATH)
        message(FATAL_ERROR "gcov not found.")
    endif(NOT GCOV_PATH)

    if(NOT GENHTML_PATH)
        message(FATAL_ERROR "GENHTML not found.")
    endif(NOT GENHTML_PATH)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage -U_FORTIFY_SOURCE")

    add_custom_target(coverage ALL
      COMMAND ${LCOV_PATH} -z -d ${CMAKE_CURRENT_BINARY_DIR}
      COMMAND ${LCOV_PATH} --no-external -c -i -d ${CMAKE_SOURCE_DIR} -o ${TEST_PROJECT}_base.info
      COMMAND ${TEST_PROJECT}
      COMMAND ${LCOV_PATH} --no-external -c -d ${CMAKE_SOURCE_DIR} -o ${TEST_PROJECT}_test.info
      COMMAND ${LCOV_PATH} -a ${TEST_PROJECT}_base.info -a ${TEST_PROJECT}_test.info -o ${TEST_PROJECT}_total.info
      COMMAND ${LCOV_PATH} --remove ${TEST_PROJECT}_total.info '${PROJECT_BINARY_DIR}/*' -o ${TEST_PROJECT}.profdata
      COMMAND ${GENHTML_PATH} -o ${TEST_PROJECT}-coverage ${TEST_PROJECT}.profdata
      DEPENDS ${TEST_PROJECT}
      BYPRODUCTS ${TEST_PROJECT}_base.info ${TEST_PROJECT}_test.info ${TEST_PROJECT}_total.info ${TEST_PROJECT}.profdata
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    find_program(LCOV_PATH llvm-cov)

    if(NOT LCOV_PATH)
        message(FATAL_ERROR "llvm-cov not found.")
    endif(NOT LCOV_PATH)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-instr-generate -fcoverage-mapping")

    file (GLOB SOURCES "${PROJECT_SOURCE_DIR}/*.hpp"
      "${PROJECT_SOURCE_DIR}/*.cpp"
      "${PROJECT_SOURCE_DIR}/test/*.hpp"
      "${PROJECT_SOURCE_DIR}/test/*.cpp"
      )

    # llvm-cov
    add_custom_target(${TEST_PROJECT}-ccov-preprocessing
        COMMAND LLVM_PROFILE_FILE=${TEST_PROJECT}.profraw $<TARGET_FILE:${TEST_PROJECT}>
        COMMAND llvm-profdata merge -sparse ${TEST_PROJECT}.profraw -o ${TEST_PROJECT}.profdata
        DEPENDS ${TEST_PROJECT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    add_custom_target(${TEST_PROJECT}-ccov-show
        COMMAND llvm-cov show $<TARGET_FILE:${TEST_PROJECT}> -instr-profile=${TEST_PROJECT}.profdata -show-line-counts-or-regions
        DEPENDS ${TEST_PROJECT}-ccov-preprocessing
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    add_custom_target(${TEST_PROJECT}-ccov-report
        COMMAND llvm-cov report $<TARGET_FILE:${TEST_PROJECT}> -instr-profile=${TEST_PROJECT}.profdata ${SOURCES}
        DEPENDS ${TEST_PROJECT}-ccov-preprocessing
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    add_custom_target(coverage ALL
        COMMAND llvm-cov show $<TARGET_FILE:${TEST_PROJECT}> -instr-profile=${TEST_PROJECT}.profdata -show-line-counts-or-regions -output-dir=${CMAKE_CURRENT_BINARY_DIR}/${TEST_PROJECT}-coverage -format="html"
        DEPENDS ${TEST_PROJECT}-ccov-report
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    endif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
endif(BUILD_COVERAGE)
