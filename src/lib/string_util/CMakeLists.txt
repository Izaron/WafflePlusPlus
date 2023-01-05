include_directories(${LLVM_INCLUDE_DIRS})
add_library(string_util STATIC string_util.cpp)

enable_testing()
add_executable(string_util_test string_util_test.cpp)
target_link_libraries(string_util_test gtest_main string_util clangTooling)
include(GoogleTest)
gtest_discover_tests(string_util_test)
