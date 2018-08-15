#-------------------------------------------------------------------------------
# hfsm_test target
#-------------------------------------------------------------------------------
add_executable(hfsm_test
			   main.cpp
			   deprecated.cpp
			   test_internal_transitions.cpp
			   test_lib_list.cpp
			   test_planner.cpp
			   test_shared.cpp
			   test_state_data.cpp)
target_link_libraries(hfsm_test hfsm)
add_dependencies(hfsm_test hfsm)
target_include_directories(hfsm_test PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../external")

#-------------------------------------------------------------------------------
# Add tests
#-------------------------------------------------------------------------------
add_test(NAME hfsm_test COMMAND hfsm_test)
add_custom_command(TARGET hfsm_test
                   POST_BUILD
                   COMMAND hfsm_test)
