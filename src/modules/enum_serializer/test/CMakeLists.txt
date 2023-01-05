# generate new files
include(waffle)
waffle_add_executable(enum_serializer_wafflec enum_serializer)
waffle_generate(
    enum_serializer_wafflec
    misc_enum_places/misc_enum_places.h
    ${CMAKE_CURRENT_SOURCE_DIR}/misc_enum_places/misc_enum_places.enum_serializer.cpp)
waffle_generate(
    enum_serializer_wafflec
    custom_names/custom_names.h
    ${CMAKE_CURRENT_SOURCE_DIR}/custom_names/custom_names.enum_serializer.cpp)

# link new files with test
add_executable(enum_serializer_test
    test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/misc_enum_places/misc_enum_places.enum_serializer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/custom_names/custom_names.enum_serializer.cpp)
target_link_libraries(enum_serializer_test gtest_main)

include(GoogleTest)
gtest_discover_tests(enum_serializer_test)
enable_testing()
