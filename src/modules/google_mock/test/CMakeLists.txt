# generate new files
include(waffle)
waffle_add_executable(google_mock_wafflec google_mock)
waffle_generate(
    google_mock_wafflec
    turtle/turtle.h
    ${CMAKE_CURRENT_SOURCE_DIR}/turtle/turtle.gmock.h)

# link new files with test
add_executable(google_mock_test
    test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/turtle/turtle.gmock.h)
target_link_libraries(google_mock_test gtest_main gmock_main)

include(GoogleTest)
gtest_discover_tests(google_mock_test)
enable_testing()
