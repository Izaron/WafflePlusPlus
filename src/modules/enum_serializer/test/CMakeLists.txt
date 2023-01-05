file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/null.cpp" "")
add_executable(enum_serializer_wafflec null.cpp)
target_link_libraries(enum_serializer_wafflec driver module file comment string_util enum_serializer clangTooling)

# copy from xxx.h (in source dir) to xxx.h.cpp (in binary dir)
add_custom_command(
  OUTPUT
    "${CMAKE_CURRENT_BINARY_DIR}/misc_enum_places/misc_enum_places.h.cpp"
  COMMAND
    ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/misc_enum_places/misc_enum_places.h"
    "${CMAKE_CURRENT_BINARY_DIR}/misc_enum_places/misc_enum_places.h.cpp"
  COMMAND
    ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/misc_enum_places/misc_enum_places.h"
    "${CMAKE_CURRENT_BINARY_DIR}/misc_enum_places/misc_enum_places.h"
  DEPENDS
    misc_enum_places/misc_enum_places.h
  VERBATIM)

# link xxx.h.cpp to a library so it will compile
add_library(misc_enum_places_header_cpp misc_enum_places/misc_enum_places.h.cpp)

# add command to codegen files
add_custom_command(
  OUTPUT
    ${CMAKE_CURRENT_SOURCE_DIR}/misc_enum_places/misc_enum_places.enum_serializer.cpp
  COMMAND
    enum_serializer_wafflec
    ${CMAKE_CURRENT_BINARY_DIR}
    misc_enum_places/misc_enum_places.h.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}
    > wafflec.out 2> wafflec.err
  DEPENDS
    enum_serializer_wafflec misc_enum_places_header_cpp
  COMMENT
    "Running wafflec generator on misc_enum_places/misc_enum_places.h"
  VERBATIM)

# link executable with codegen files
add_executable(enum_serializer_test
  test.cpp
  misc_enum_places/misc_enum_places.enum_serializer.cpp)
target_link_libraries(enum_serializer_test gtest_main)

include(GoogleTest)
gtest_discover_tests(enum_serializer_test)
enable_testing()
