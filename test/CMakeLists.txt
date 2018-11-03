#-------------------------------------------------------------------------------
# hfsm2_test target
#-------------------------------------------------------------------------------
add_executable(hfsm2_test
			   deprecated.cpp
			   main.cpp
			   test_internal_transitions.cpp
			   test_lib_bit_array.cpp
			   test_lib_list.cpp
			   test_lib_object_pool.cpp
			   test_planner.cpp
			   test_shared.cpp
			   test_state_data.cpp)
target_link_libraries(hfsm2_test hfsm2)
add_dependencies(hfsm2_test hfsm2)
target_include_directories(hfsm2_test PRIVATE "${CMAKE_CURRENT_LIST_DIR}/../external")

#-------------------------------------------------------------------------------
# Add tests
#-------------------------------------------------------------------------------
add_test(NAME hfsm2_test COMMAND hfsm2_test)
add_custom_command(TARGET hfsm2_test
				   POST_BUILD
				   COMMAND hfsm2_test)
