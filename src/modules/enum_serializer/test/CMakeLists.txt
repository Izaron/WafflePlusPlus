file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/null.cpp" "")
add_executable(enum_serializer_wafflec null.cpp)
target_link_libraries(enum_serializer_wafflec driver module file enum_serializer clangTooling)

add_custom_target(
  enum_serializer_generate_code
  COMMAND enum_serializer_wafflec "${CMAKE_CURRENT_SOURCE_DIR}/comment_styles"
  COMMENT "Running wafflec generator on ./comment_styles"
  DEPENDS enum_serializer_wafflec
  VERBATIM)

file(GLOB_RECURSE TEST_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
add_executable(enum_serializer_test test.cpp ${TEST_SOURCES})
target_link_libraries(enum_serializer_test gtest_main)
add_dependencies(enum_serializer_test enum_serializer_generate_code)

include(GoogleTest)
gtest_discover_tests(enum_serializer_test)
enable_testing()
