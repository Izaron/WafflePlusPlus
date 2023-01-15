list(APPEND MODULES signals)

# generate new files
include(waffle)
waffle_add_executable(signals_wafflec "${MODULES}")
waffle_generate(
    signals_wafflec
    counter/counter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/counter/counter.signals.cpp)

# link new files with test
add_executable(signals_test
    test.cpp
    counter/counter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/counter/counter.signals.cpp)
target_link_libraries(signals_test gtest_main)

include(GoogleTest)
gtest_discover_tests(signals_test)
enable_testing()
