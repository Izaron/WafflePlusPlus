file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/null.cpp" "")
add_executable(enum_serializer_wafflec null.cpp)
target_link_libraries(enum_serializer_wafflec driver module file enum_serializer clangTooling)

add_custom_target(
  enum_serializer_generate_comment_styles
  COMMAND enum_serializer_wafflec "${CMAKE_CURRENT_BINARY_DIR}/comment_styles/comment_styles.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/comment_styles"
  COMMENT "Running wafflec generator on ${CMAKE_CURRENT_BINARY_DIR}/comment_styles/comment_styles.cpp"
  DEPENDS enum_serializer_wafflec
  VERBATIM)

#add_custom_target(
  #enum_serializer_generate_custom_names
  #COMMAND enum_serializer_wafflec "${CMAKE_CURRENT_SOURCE_DIR}/custom_names"
  #COMMENT "Running wafflec generator on ./custom_names"
  #DEPENDS enum_serializer_wafflec
  #VERBATIM)

file(GLOB_RECURSE TEST_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
add_executable(enum_serializer_test test.cpp ${TEST_SOURCES})
target_link_libraries(enum_serializer_test gtest_main)
#add_dependencies(enum_serializer_test enum_serializer_generate_comment_styles enum_serializer_generate_custom_names)
add_dependencies(enum_serializer_test enum_serializer_generate_comment_styles)

include(GoogleTest)
gtest_discover_tests(enum_serializer_test)
enable_testing()
