set(GENERATED_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/misc_enum_places/misc_enum_places.enum_serializer.cpp)

# generate new files
include(waffle)
waffle_add_executable(enum_serializer_wafflec enum_serializer)
waffle_generate(
    enum_serializer_wafflec
    misc_enum_places/misc_enum_places.h
    ${GENERATED_FILES})

# link new files with test
add_executable(enum_serializer_test test.cpp ${GENERATED_FILES})
target_link_libraries(enum_serializer_test gtest_main)

include(GoogleTest)
gtest_discover_tests(enum_serializer_test)
enable_testing()
