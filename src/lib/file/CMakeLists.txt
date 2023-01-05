add_library(file STATIC file.cpp nice_printer.cpp)

enable_testing()
add_executable(file_test file_test.cpp)
target_link_libraries(file_test gtest_main file)
include(GoogleTest)
gtest_discover_tests(file_test)
