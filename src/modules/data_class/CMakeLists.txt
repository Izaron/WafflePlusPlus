include_directories(${LLVM_INCLUDE_DIRS})
waffle_generate_template_data()
add_library(data_class OBJECT register.cpp collector.cpp printer.cpp template.cpp.data)
target_include_directories(data_class PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
add_subdirectory(test)
enable_testing()
