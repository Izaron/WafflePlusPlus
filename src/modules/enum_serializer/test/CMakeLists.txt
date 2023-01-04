file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/null.cpp" "")
add_executable(enum_serializer_wafflec null.cpp)
target_link_libraries(enum_serializer_wafflec driver module file enum_serializer clangTooling)

add_custom_command(
  OUTPUT enum_serializer_generate_comment_styles_marker
  COMMAND touch enum_serializer_generate_comment_styles_marker
  COMMAND enum_serializer_wafflec "${CMAKE_CURRENT_BINARY_DIR}/comment_styles/comment_styles.cpp" "${CMAKE_CURRENT_BINARY_DIR}/comment_styles" > wafflec_out 2> wafflec_err
  DEPENDS enum_serializer_wafflec "${CMAKE_CURRENT_SOURCE_DIR}/comment_styles/comment_styles.cpp"
  COMMENT "Running wafflec generator on ${CMAKE_CURRENT_BINARY_DIR}/comment_styles/comment_styles.cpp"
  VERBATIM)

add_custom_target(
  enum_serializer_generate_comment_styles
  DEPENDS enum_serializer_generate_comment_styles_marker
)

file(GLOB_RECURSE TEST_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
add_executable(enum_serializer_test test.cpp ${TEST_SOURCES})
target_link_libraries(enum_serializer_test gtest_main)
add_dependencies(enum_serializer_test enum_serializer_generate_comment_styles)

include(GoogleTest)
gtest_discover_tests(enum_serializer_test)
enable_testing()
