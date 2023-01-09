# generate new files
include(waffle)
waffle_add_executable(data_class_wafflec data_class)
waffle_generate(
    data_class_wafflec
    mountains/mountains.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mountains/mountains.data_class.h)

# link new files with test
add_executable(data_class_test
    test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mountains/mountains.data_class.h)
target_link_libraries(data_class_test gtest_main)

include(GoogleTest)
gtest_discover_tests(data_class_test)
enable_testing()
