include_directories(${LLVM_INCLUDE_DIRS})
add_executable(enum_serializer module.cpp)
target_link_libraries(enum_serializer clangTooling)
