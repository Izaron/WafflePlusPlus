file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/null.cpp" "")
add_executable(wafflec null.cpp)
target_link_libraries(wafflec driver module file comment string_util enum_serializer clangTooling)
