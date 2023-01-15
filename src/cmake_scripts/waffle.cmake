function(waffle_add_executable WAFFLEC_BINARY MODULES)
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/null.cpp" "")
  add_executable(${WAFFLEC_BINARY} null.cpp)
  target_link_libraries(${WAFFLEC_BINARY} driver file registry comment string_util clangTooling ${MODULES})
endfunction()

function(waffle_generate WAFFLEC_BINARY HEADER GENERATED_FILES)
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
  set(HEADER_STR "${CMAKE_CURRENT_BINARY_DIR}/${HEADER}_cpp")
  string(REPLACE "/" "_" HEADER_STR ${HEADER_STR})
  add_library(${HEADER_STR} "${HEADER}.cpp")

  # defining the output directory
  string(REGEX MATCH "^(${CMAKE_CURRENT_SOURCE_DIR}|${CMAKE_CURRENT_BINARY_DIR})" OUTPUT_DIR "${GENERATED_FILES}")

  # add command to codegen files
  add_custom_command(
    OUTPUT
      ${GENERATED_FILES}
    COMMAND
      ${WAFFLEC_BINARY}            # argv[0] == wafllec binary
      ${CMAKE_CURRENT_BINARY_DIR}  # argv[1] == current binary dir
      "${HEADER}.cpp"              # argv[2] == path to source .cpp file
      ${OUTPUT_DIR}                # argv[3] == dir where to save generated files
      > "${HEADER}.wafflec.out" 2> "${HEADER}.wafflec.err"
    DEPENDS
      ${WAFFLEC_BINARY} ${HEADER_STR}
    COMMENT
      "Running wafflec generator on ${HEADER}"
    VERBATIM)
endfunction()

function(waffle_generate_template_data)
  add_custom_command(
    OUTPUT
      template.cpp.data
    COMMAND
      xxd -i template.cpp ${CMAKE_CURRENT_BINARY_DIR}/template.cpp.data
    WORKING_DIRECTORY
      ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS
      template.cpp
    COMMENT
      "Generate template.cpp.data for ${CMAKE_CURRENT_SOURCE_DIR}"
  )
endfunction()
