add_library(file STATIC file.cpp)

enable_testing()
add_executable(file_test file_test.cpp)
target_link_libraries(file_test gtest_main)
include(GoogleTest)
gtest_discover_tests(file_test)
