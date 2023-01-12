# generate new files
include(waffle)
waffle_add_executable(poly_ptr_wafflec poly_ptr)
waffle_generate(
    poly_ptr_wafflec
    structs/structs.h
    ${CMAKE_CURRENT_SOURCE_DIR}/structs/structs.poly_ptr.h)

# link new files with test
add_executable(poly_ptr_test
    poly_obj_test.cpp
    poly_ref_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/structs/structs.poly_ptr.h)
target_link_libraries(poly_ptr_test gtest_main)

include(GoogleTest)
gtest_discover_tests(poly_ptr_test)
enable_testing()
