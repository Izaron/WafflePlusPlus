find_package(Clang REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Found Clang ${Clang_PACKAGE_VERSION}")

include_directories(${LLVM_INCLUDE_DIRS})
add_executable(enum_serializer module.cpp)
target_link_libraries(enum_serializer clangTooling)
