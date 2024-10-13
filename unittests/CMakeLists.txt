include(GoogleTest)

add_executable(sliding_window_unittest
    ${CMAKE_CURRENT_SOURCE_DIR}/sliding_window_unittest.cpp
)

target_link_libraries(sliding_window_unittest PRIVATE
    sliding_window
    GTest::gtest_main
    slidingwindow_compiler_flags
)

gtest_discover_tests(sliding_window_unittest)
