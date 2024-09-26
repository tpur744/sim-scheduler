option(PART_1_TEST_CASES "Run the test cases for Task 1" ON)
option(PART_2_TEST_CASES "Run the test cases for Task 2" ON)

# Add the test executable and link it with the library targets
add_executable(tester_app ./tester.cpp)

# Include the "src" directory to access the source files
target_include_directories(tester_app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src)

# Link the subdirectory targets to the test executable
target_link_libraries(tester_app SrcTarget SrcAppSupportTarget)

if (PART_1_TEST_CASES)
  add_test(NAME T1 COMMAND tester_app 1)
  add_test(NAME T2 COMMAND tester_app 2)
  add_test(NAME T3 COMMAND tester_app 3)
  add_test(NAME T4 COMMAND tester_app 4)
endif()

if (PART_2_TEST_CASES)
  add_test(NAME T5 COMMAND tester_app 5)
  add_test(NAME T6 COMMAND tester_app 6)
  add_test(NAME T7 COMMAND tester_app 7)
  add_test(NAME T8 COMMAND tester_app 8)
  add_test(NAME T9 COMMAND tester_app 9)
  add_test(NAME T10 COMMAND tester_app 10)
  add_test(NAME T11 COMMAND tester_app 11)
  add_test(NAME T12 COMMAND tester_app 12)
endif()

