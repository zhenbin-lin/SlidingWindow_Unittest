#include <gtest/gtest.h>
#include <stddef.h>

#include "sliding_window.h"

#include <stdint.h>

uint8_t window_buffer[64];

TEST(SlidingWindowTest, write_test)
{
    sliding_window window;
    sliding_window_init(&window, window_buffer, sizeof(window_buffer), sizeof(uint8_t));

    sliding_window_write(&window, (uint8_t *) "Hello, World!", 13);
    sliding_window_write(&window, (uint8_t *) "Hello, World!", 13);
    sliding_window_write(&window, (uint8_t *) "Hello, World!", 13);
    sliding_window_write(&window, (uint8_t *) "Hello, World!", 13);
    EXPECT_EQ(sliding_window_active_count(&window), 13 * 4);

    sliding_window_write(&window, (uint8_t *) "Hello, World!", 13);
    EXPECT_EQ(sliding_window_active_count(&window), 63);
    EXPECT_EQ(sliding_window_available_count(&window), 0);
    sliding_window_write(&window, (uint8_t *) "Hello, World!", 13);
    EXPECT_EQ(sliding_window_active_count(&window), 63);
    EXPECT_EQ(sliding_window_available_count(&window), 0);

    sliding_window_drop(&window, 13);
    sliding_window_drop(&window, 13);
    sliding_window_drop(&window, 13);
    EXPECT_EQ(sliding_window_active_count(&window), 63);

    sliding_window_slide(&window, 13);
    sliding_window_slide(&window, 13);
    sliding_window_slide(&window, 13);
    EXPECT_EQ(sliding_window_active_count(&window), 24);

    sliding_window_write(&window, (uint8_t *) "Hello, World!", 13);
    sliding_window_write(&window, (uint8_t *) "Hello, World!", 13);
    sliding_window_write(&window, (uint8_t *) "Hello, World!", 13);
    EXPECT_EQ(sliding_window_active_count(&window), 24);

    sliding_window_drop(&window, 13);
    sliding_window_drop(&window, 13);
    EXPECT_EQ(sliding_window_available_count(&window), 26);
    sliding_window_write(&window, (uint8_t *) "Hello, World!", 13);
    sliding_window_write(&window, (uint8_t *) "Hello, World!", 13);
    EXPECT_EQ(sliding_window_active_count(&window), 50);
}

TEST(SlidingWindowTest, read_test)
{
    sliding_window window; 
    sliding_window_init(&window, window_buffer, sizeof(window_buffer), sizeof(uint8_t));
    char rx_buffer[128] = {0};

    sliding_window_write(&window, (uint8_t *) "Hello, World!", 13);
    sliding_window_write(&window, (uint8_t *) "Hello, World!", 13);
    sliding_window_write(&window, (uint8_t *) "Hello, World!", 13);
    sliding_window_write(&window, (uint8_t *) "Hello, World!", 13);
    sliding_window_write(&window, (uint8_t *) "Hello, World!", 13);
    sliding_window_write(&window, (uint8_t *) "Hello, World!", 13);

    sliding_window_read(&window, rx_buffer, 13);
    sliding_window_read(&window, rx_buffer, 13);
    sliding_window_read(&window, rx_buffer, 13);
    EXPECT_EQ(sliding_window_active_count(&window), 63);
    EXPECT_STREQ(rx_buffer, "Hello, World!");

    sliding_window_slide(&window, 13);
    sliding_window_slide(&window, 13);
    EXPECT_EQ(sliding_window_active_count(&window), 37);
    EXPECT_EQ(sliding_window_cache_count(&window), 26);

    sliding_window_drop(&window, 13);
    EXPECT_EQ(sliding_window_active_count(&window), 37);
    EXPECT_EQ(sliding_window_cache_count(&window), 13);
    sliding_window_drop(&window, 13);
    EXPECT_EQ(sliding_window_active_count(&window), 37);
    EXPECT_EQ(sliding_window_cache_count(&window), 0);
}

