add_library(sliding_window STATIC)

target_sources(sliding_window PRIVATE
    src/sliding_window.c
)

target_link_libraries(
    sliding_window
    slidingwindow_compiler_flags
)

target_include_directories(sliding_window PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
