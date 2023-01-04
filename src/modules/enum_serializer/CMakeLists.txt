include_directories(${LLVM_INCLUDE_DIRS})
add_library(enum_serializer OBJECT module.cpp)
add_subdirectory(test)
enable_testing()
