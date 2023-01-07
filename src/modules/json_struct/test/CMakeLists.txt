# generate new files
include(waffle)
waffle_add_executable(json_struct_wafflec json_struct)
waffle_generate(
    json_struct_wafflec
    simple_structs/simple_structs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/simple_structs/simple_structs.json_struct.cpp)

# link new files with test
add_executable(json_struct_test
    test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/simple_structs/simple_structs.json_struct.cpp)
target_link_libraries(json_struct_test gtest_main)

include(GoogleTest)
gtest_discover_tests(json_struct_test)
enable_testing()
