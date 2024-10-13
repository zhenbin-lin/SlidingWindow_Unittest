cmake_minimum_required(VERSION 3.20.0)

project(SlidingWindow)

enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(slidingwindow_compiler_flags INTERFACE)

target_compile_features(slidingwindow_compiler_flags INTERFACE cxx_std_17)
target_compile_features(slidingwindow_compiler_flags INTERFACE c_std_99)
target_compile_options(slidingwindow_compiler_flags INTERFACE -Wall -Wextra)

if (CMAKE_BUILD_TYPE STRLESS_EQUAL "Release")
    target_compile_options(slidingwindow_compiler_flags INTERFACE -O2)
elseif ((CMAKE_BUILD_TYPE STRLESS_EQUAL "Debug"))
    target_compile_options(slidingwindow_compiler_flags INTERFACE -O0 -g -fPIC)
endif()

add_subdirectory(sliding_window)
add_subdirectory(unittests)
add_subdirectory(third_party/googletest)
