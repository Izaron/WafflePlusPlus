include_directories(${LLVM_INCLUDE_DIRS})
add_library(dump_struct OBJECT module.cpp collector.cpp printer.cpp)
add_subdirectory(test)
enable_testing()
