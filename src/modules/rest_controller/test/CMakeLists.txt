list(APPEND MODULES rest_controller json_dump)

list(APPEND GENERATED_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/employee/employee.rest_controller.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/employee/employee.json_dump.cpp)

# generate new files
include(waffle)
waffle_add_executable(rest_controller_wafflec "${MODULES}")
waffle_generate(
    rest_controller_wafflec
    employee/employee.h
    "${GENERATED_FILES}")

# link new files with test
add_executable(rest_controller_test
    test.cpp
    ${GENERATED_FILES})
target_link_libraries(rest_controller_test string_util clangTooling gtest_main gmock_main)

include(GoogleTest)
gtest_discover_tests(rest_controller_test)
enable_testing()
