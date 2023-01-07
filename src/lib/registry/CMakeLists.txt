add_library(registry OBJECT registry.cpp)

enable_testing()
add_executable(module_test registry_test.cpp)
target_link_libraries(module_test gtest_main registry)
include(GoogleTest)
gtest_discover_tests(module_test)
