include_directories(${LLVM_INCLUDE_DIRS})
add_library(string_util STATIC string_util.cpp)

#enable_testing()
#add_executable(comment_test comment_test.cpp)
#target_link_libraries(comment_test gtest_main comment clangTooling)
#include(GoogleTest)
#gtest_discover_tests(comment_test)
