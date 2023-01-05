file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/null.cpp" "")
add_executable(enum_serializer_wafflec null.cpp)
target_link_libraries(enum_serializer_wafflec driver module file comment string_util enum_serializer clangTooling)

function(waffle_generate HEADER WAFFLEC_BINARY GENERATED_FILES)
  # copy from xxx.h (in source dir) to xxx.h.cpp (in binary dir)
  add_custom_command(
    OUTPUT
      "${CMAKE_CURRENT_BINARY_DIR}/${HEADER}.cpp"
    COMMAND
      ${CMAKE_COMMAND} -E copy
      "${CMAKE_CURRENT_SOURCE_DIR}/${HEADER}"
      "${CMAKE_CURRENT_BINARY_DIR}/${HEADER}.cpp"
    COMMAND
      ${CMAKE_COMMAND} -E copy
      "${CMAKE_CURRENT_SOURCE_DIR}/${HEADER}"
      "${CMAKE_CURRENT_BINARY_DIR}/${HEADER}"
    DEPENDS
      ${HEADER}
    VERBATIM)

  # link xxx.h.cpp to a library so it will compile
  string(REPLACE "/" "_" HEADER_STR ${HEADER})
  add_library("${HEADER_STR}_cpp" "${HEADER}.cpp")

  # add command to codegen files
  add_custom_command(
    OUTPUT
      ${GENERATED_FILES}
    COMMAND
      ${WAFFLEC_BINARY}
      ${CMAKE_CURRENT_BINARY_DIR}
      "${HEADER}.cpp"
      ${CMAKE_CURRENT_SOURCE_DIR}
      > "${HEADER}.wafflec.out" 2> "${HEADER}.wafflec.err"
    DEPENDS
      ${WAFFLEC_BINARY} "${HEADER_STR}_cpp"
    COMMENT
      "Running wafflec generator on ${HEADER}"
    VERBATIM)
endfunction()

set(GENERATED_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/misc_enum_places/misc_enum_places.enum_serializer.cpp)

waffle_generate(
    misc_enum_places/misc_enum_places.h
    enum_serializer_wafflec
    ${GENERATED_FILES})

# link executable with codegen files
add_executable(enum_serializer_test
  test.cpp
  ${GENERATED_FILES})
target_link_libraries(enum_serializer_test gtest_main)

include(GoogleTest)
gtest_discover_tests(enum_serializer_test)
enable_testing()
