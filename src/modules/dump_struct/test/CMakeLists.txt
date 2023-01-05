# generate new files
include(waffle)
waffle_add_executable(dump_struct_wafflec dump_struct)
waffle_generate(
    dump_struct_wafflec
    simple_structs/simple_structs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/simple_structs/simple_structs.dump_struct.cpp)

# link new files with test
add_executable(dump_struct_test
    test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/simple_structs/simple_structs.dump_struct.cpp)
target_link_libraries(dump_struct_test gtest_main)

include(GoogleTest)
gtest_discover_tests(dump_struct_test)
enable_testing()
