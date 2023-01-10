# generate new files
include(waffle)
waffle_add_executable(rest_controller_wafflec rest_controller)
waffle_generate(
    rest_controller_wafflec
    employee/employee.h
    ${CMAKE_CURRENT_SOURCE_DIR}/employee/employee.rest_controller.cpp)

# link new files with test
add_executable(rest_controller_test
    test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/employee/employee.rest_controller.cpp)
target_link_libraries(rest_controller_test gtest_main gmock_main)

include(GoogleTest)
gtest_discover_tests(rest_controller_test)
enable_testing()
