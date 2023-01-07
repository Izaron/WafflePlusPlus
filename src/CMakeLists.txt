cmake_minimum_required(VERSION 3.14)

project(WafflePlusPlus VERSION 0.1 LANGUAGES C CXX)

# compiler settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-enum-enum-conversion")
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/include)

# additional cmake scripts
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake_scripts")

# include clang
find_package(Clang REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Found Clang ${Clang_PACKAGE_VERSION}")

# include jinja2cpp
set(jinja2cpp_DIR "/home/izaron/jinja2cpp/lib/jinja2cpp")
find_package(jinja2cpp REQUIRED)

# add subdirectories
add_subdirectory(bin)
add_subdirectory(lib)
add_subdirectory(modules)

# enable googletest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.0
)
FetchContent_MakeAvailable(googletest)
enable_testing()
