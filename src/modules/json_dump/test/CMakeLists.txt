# generate new files
include(waffle)
waffle_add_executable(json_dump_wafflec json_dump)
waffle_generate(
    json_dump_wafflec
    books_library/books_library.h
    ${CMAKE_CURRENT_SOURCE_DIR}/books_library/books_library.json_dump.cpp)

# link new files with test
add_executable(json_dump_test
    test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/books_library/books_library.json_dump.cpp)
target_link_libraries(json_dump_test gtest_main)

include(GoogleTest)
gtest_discover_tests(json_dump_test)
enable_testing()
