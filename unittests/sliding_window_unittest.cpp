#include "gtest/gtest.h"
#include <cmath>
#include <cstring>
#include <gtest/gtest.h>
#include <stddef.h>

#include "sliding_window.h"

#include <stdint.h>

uint8_t window_buffer[64];

TEST(SlidingWindowTest, tx_window_write_test)
{
    sliding_window window;
    sliding_window_init(&window, window_buffer, sizeof(window_buffer), sizeof(uint8_t), true);

    EXPECT_EQ(sliding_window_data_count(&window), 0);
    EXPECT_EQ(sliding_window_cache_count(&window), 0);
    EXPECT_EQ(sliding_window_active_count(&window), 0);
    EXPECT_EQ(sliding_window_await_count(&window), 0);
    EXPECT_EQ(sliding_window_available_count(&window), 63);

    sliding_window_write(&window, (uint8_t *) "Hello, World!", 13);
    sliding_window_write(&window, (uint8_t *) "Hello, World!", 13);
    sliding_window_write(&window, (uint8_t *) "Hello, World!", 13);
    sliding_window_write(&window, (uint8_t *) "Hello, World!", 13);

    EXPECT_EQ(sliding_window_data_count(&window), 52);
    EXPECT_EQ(sliding_window_cache_count(&window), 0);
    EXPECT_EQ(sliding_window_active_count(&window), 0);
    EXPECT_EQ(sliding_window_await_count(&window), 52);
    EXPECT_EQ(sliding_window_available_count(&window), 11);

    sliding_window_slide(&window, 13);
    sliding_window_slide(&window, 13);
    sliding_window_slide(&window, 13);
    sliding_window_slide(&window, 13);
    EXPECT_EQ(sliding_window_cache_count(&window), 52);
    EXPECT_EQ(sliding_window_active_count(&window), 0);
    EXPECT_EQ(sliding_window_await_count(&window), 0);
    EXPECT_EQ(sliding_window_available_count(&window), 11);


    sliding_window_drop(&window, 13);
    sliding_window_drop(&window, 13);
    EXPECT_EQ(sliding_window_cache_count(&window), 26);
    EXPECT_EQ(sliding_window_active_count(&window), 0);
    EXPECT_EQ(sliding_window_await_count(&window), 0);
    EXPECT_EQ(sliding_window_available_count(&window), 37);

    sliding_window_write(&window, (uint8_t *) "Hello, World!", 13);
    sliding_window_write(&window, (uint8_t *) "Hello, World!", 13);
    EXPECT_EQ(sliding_window_cache_count(&window), 26);
    EXPECT_EQ(sliding_window_active_count(&window), 0);
    EXPECT_EQ(sliding_window_await_count(&window), 26);
    EXPECT_EQ(sliding_window_available_count(&window), 11);

    sliding_window_slide(&window, 13);
    sliding_window_slide(&window, 13);
    sliding_window_slide(&window, 13);
    sliding_window_slide(&window, 13);
    EXPECT_EQ(sliding_window_cache_count(&window), 52);
    EXPECT_EQ(sliding_window_active_count(&window), 0);
    EXPECT_EQ(sliding_window_await_count(&window), 0);
    EXPECT_EQ(sliding_window_available_count(&window), 11);


    sliding_window_drop(&window, 13);
    sliding_window_drop(&window, 13);
    sliding_window_drop(&window, 13);
    sliding_window_drop(&window, 13);
    sliding_window_drop(&window, 13);
    sliding_window_drop(&window, 13);
    EXPECT_EQ(sliding_window_data_count(&window), 0);
    EXPECT_EQ(sliding_window_cache_count(&window), 0);
    EXPECT_EQ(sliding_window_active_count(&window), 0);
    EXPECT_EQ(sliding_window_await_count(&window), 0);
    EXPECT_EQ(sliding_window_available_count(&window), 63);
}

TEST(SlidingWindowTest, tx_window_read_test)
{
    sliding_window window; 
    sliding_window_init(&window, window_buffer, sizeof(window_buffer), sizeof(uint8_t), true);
    char rx_buffer[128] = {0};

    sliding_window_write(&window, (uint8_t *) "Hello, World!", 13);
    sliding_window_write(&window, (uint8_t *) "Hello, World!", 13);
    sliding_window_write(&window, (uint8_t *) "Hello, World!", 13);
    sliding_window_write(&window, (uint8_t *) "Hello, World!", 13);
    sliding_window_write(&window, (uint8_t *) "Hello, World!", 13);
    sliding_window_write(&window, (uint8_t *) "Hello, World!", 13);
    EXPECT_EQ(sliding_window_data_count(&window), 63);
    EXPECT_EQ(sliding_window_cache_count(&window), 0);
    EXPECT_EQ(sliding_window_active_count(&window), 0);
    EXPECT_EQ(sliding_window_await_count(&window), 63);
    EXPECT_EQ(sliding_window_available_count(&window), 0);

    sliding_window_read(&window, rx_buffer, 13);
    sliding_window_read(&window, rx_buffer, 13);
    sliding_window_read(&window, rx_buffer, 13);
    EXPECT_EQ(sliding_window_data_count(&window), 63);
    EXPECT_EQ(sliding_window_cache_count(&window), 0);
    EXPECT_EQ(sliding_window_active_count(&window), 0);
    EXPECT_EQ(sliding_window_await_count(&window), 63);
    EXPECT_EQ(sliding_window_available_count(&window), 0);

    sliding_window_slide(&window, 13);
    sliding_window_slide(&window, 13);
    sliding_window_read(&window, rx_buffer, 13);
    sliding_window_read(&window, rx_buffer, 13);
    sliding_window_read(&window, rx_buffer, 13);
    EXPECT_EQ(sliding_window_data_count(&window), 63);
    EXPECT_EQ(sliding_window_cache_count(&window), 26);
    EXPECT_EQ(sliding_window_active_count(&window), 0);
    EXPECT_EQ(sliding_window_await_count(&window), 37);
    EXPECT_EQ(sliding_window_available_count(&window), 0);


    sliding_window_dilate(&window, 1);
    EXPECT_EQ(sliding_window_read(&window, rx_buffer, 13), 1);
    sliding_window_dilate(&window, 12);
    EXPECT_EQ(sliding_window_read(&window, rx_buffer, 13), 13);
    EXPECT_STREQ(rx_buffer, "Hello, World!");
    EXPECT_EQ(sliding_window_data_count(&window), 63);
    EXPECT_EQ(sliding_window_cache_count(&window), 26);
    EXPECT_EQ(sliding_window_active_count(&window), 13);
    EXPECT_EQ(sliding_window_await_count(&window), 24);
    EXPECT_EQ(sliding_window_available_count(&window), 0);


    EXPECT_EQ(sliding_window_slide(&window, 1), 1);
    EXPECT_EQ(sliding_window_read(&window, rx_buffer, 13), 13);
    EXPECT_STREQ(rx_buffer, "ello, World!H");
    EXPECT_EQ(sliding_window_slide(&window, 12), 12);
    EXPECT_EQ(sliding_window_read(&window, rx_buffer, 13), 13);
    EXPECT_STREQ(rx_buffer, "Hello, World!");

    EXPECT_EQ(sliding_window_data_count(&window), 63);
    EXPECT_EQ(sliding_window_cache_count(&window), 39);
    EXPECT_EQ(sliding_window_active_count(&window), 13);
    EXPECT_EQ(sliding_window_await_count(&window), 11);
    EXPECT_EQ(sliding_window_available_count(&window), 0);

    memset(rx_buffer, 0, sizeof(rx_buffer));
    sliding_window_shrink(&window, 1);
    EXPECT_EQ(sliding_window_read(&window, rx_buffer, 13), 12);
    EXPECT_STREQ(rx_buffer, "ello, World!");

    memset(rx_buffer, 0, sizeof(rx_buffer));
    sliding_window_shrink(&window, 1);
    EXPECT_EQ(sliding_window_read(&window, rx_buffer, 13), 11);
    EXPECT_STREQ(rx_buffer, "llo, World!");

    memset(rx_buffer, 0, sizeof(rx_buffer));
    sliding_window_shrink(&window, 1);
    EXPECT_EQ(sliding_window_read(&window, rx_buffer, 13), 10);
    EXPECT_STREQ(rx_buffer, "lo, World!");

    memset(rx_buffer, 0, sizeof(rx_buffer));
    sliding_window_shrink(&window, 4);
    EXPECT_EQ(sliding_window_read(&window, rx_buffer, 13), 6);
    EXPECT_STREQ(rx_buffer, "World!");

    EXPECT_EQ(sliding_window_data_count(&window), 63);
    EXPECT_EQ(sliding_window_cache_count(&window), 46);
    EXPECT_EQ(sliding_window_active_count(&window), 6);
    EXPECT_EQ(sliding_window_await_count(&window), 11);
    EXPECT_EQ(sliding_window_available_count(&window), 0);


    EXPECT_EQ(sliding_window_slide(&window, 13), 11);
    EXPECT_EQ(sliding_window_data_count(&window), 63);
    EXPECT_EQ(sliding_window_cache_count(&window), 57);
    EXPECT_EQ(sliding_window_active_count(&window), 6);
    EXPECT_EQ(sliding_window_await_count(&window), 0);
    EXPECT_EQ(sliding_window_available_count(&window), 0);
    memset(rx_buffer, 0, sizeof(rx_buffer));
    EXPECT_EQ(sliding_window_read(&window, rx_buffer, 13), 6);
    EXPECT_STREQ(rx_buffer, ", Worl");

    EXPECT_EQ(sliding_window_dilate(&window, -7), 7);
    EXPECT_EQ(sliding_window_data_count(&window), 63);
    EXPECT_EQ(sliding_window_cache_count(&window), 50);
    EXPECT_EQ(sliding_window_active_count(&window), 13);
    EXPECT_EQ(sliding_window_await_count(&window), 0);
    EXPECT_EQ(sliding_window_available_count(&window), 0);
    memset(rx_buffer, 0, sizeof(rx_buffer));
    EXPECT_EQ(sliding_window_read(&window, rx_buffer, 13), 13);
    EXPECT_STREQ(rx_buffer, "d!Hello, Worl");

    EXPECT_EQ(sliding_window_slide(&window, -13), 13);
    EXPECT_EQ(sliding_window_data_count(&window), 63);
    EXPECT_EQ(sliding_window_cache_count(&window), 37);
    EXPECT_EQ(sliding_window_active_count(&window), 13);
    EXPECT_EQ(sliding_window_await_count(&window), 13);
    EXPECT_EQ(sliding_window_available_count(&window), 0);

    memset(rx_buffer, 0, sizeof(rx_buffer));
    EXPECT_EQ(sliding_window_read(&window, rx_buffer, 13), 13);
    EXPECT_STREQ(rx_buffer, "d!Hello, Worl");

    EXPECT_EQ(sliding_window_slide(&window, 13), 13);
    EXPECT_EQ(sliding_window_slide(&window, 13), 0);
    EXPECT_EQ(sliding_window_data_count(&window), 63);
    EXPECT_EQ(sliding_window_cache_count(&window), 50);
    EXPECT_EQ(sliding_window_active_count(&window), 13);
    EXPECT_EQ(sliding_window_await_count(&window), 0);
    EXPECT_EQ(sliding_window_available_count(&window), 0);

    EXPECT_EQ(sliding_window_shrink(&window, 13), 13);
    EXPECT_EQ(sliding_window_data_count(&window), 63);
    EXPECT_EQ(sliding_window_cache_count(&window), 63);
    EXPECT_EQ(sliding_window_active_count(&window), 0);
    EXPECT_EQ(sliding_window_await_count(&window), 0);
    EXPECT_EQ(sliding_window_available_count(&window), 0);

    EXPECT_EQ(sliding_window_drop(&window, 13), 13);
    EXPECT_EQ(sliding_window_drop(&window, 13), 13);
    EXPECT_EQ(sliding_window_drop(&window, 13), 13);
    EXPECT_EQ(sliding_window_data_count(&window), 24);
    EXPECT_EQ(sliding_window_cache_count(&window), 24);
    EXPECT_EQ(sliding_window_active_count(&window), 0);
    EXPECT_EQ(sliding_window_await_count(&window), 0);
    EXPECT_EQ(sliding_window_available_count(&window), 39);

    EXPECT_EQ(sliding_window_drop(&window, 13), 13);
    EXPECT_EQ(sliding_window_drop(&window, 13), 11);
    EXPECT_EQ(sliding_window_drop(&window, 13), 0);
    EXPECT_EQ(sliding_window_data_count(&window), 0);
    EXPECT_EQ(sliding_window_cache_count(&window), 0);
    EXPECT_EQ(sliding_window_active_count(&window), 0);
    EXPECT_EQ(sliding_window_await_count(&window), 0);
    EXPECT_EQ(sliding_window_available_count(&window), 63);
}

TEST(SlidingWindowTest, tx_window_sliding_test)
{
    sliding_window window;
    sliding_window_init(&window, window_buffer, sizeof(window_buffer), sizeof(uint8_t), true);

    EXPECT_EQ(sliding_window_write(&window, (uint8_t *) "Hello, World!", 13), 13);
    EXPECT_EQ(sliding_window_write(&window, (uint8_t *) "Hello, World!", 13), 13);
    EXPECT_EQ(sliding_window_write(&window, (uint8_t *) "Hello, World!", 13), 13);
    EXPECT_EQ(sliding_window_write(&window, (uint8_t *) "Hello, World!", 13), 13);
    EXPECT_EQ(sliding_window_data_count(&window), 52);
    EXPECT_EQ(sliding_window_cache_count(&window), 0);
    EXPECT_EQ(sliding_window_active_count(&window), 0);
    EXPECT_EQ(sliding_window_await_count(&window), 52);
    EXPECT_EQ(sliding_window_available_count(&window), 11);

    EXPECT_EQ(sliding_window_dilate(&window, 13), 13);
    EXPECT_EQ(sliding_window_dilate(&window, 13), 13);
    EXPECT_EQ(sliding_window_dilate(&window, 13), 13);
    EXPECT_EQ(sliding_window_dilate(&window, 13), 13);
    EXPECT_EQ(sliding_window_data_count(&window), 52);
    EXPECT_EQ(sliding_window_cache_count(&window), 0);
    EXPECT_EQ(sliding_window_active_count(&window), 52);
    EXPECT_EQ(sliding_window_await_count(&window), 0);
    EXPECT_EQ(sliding_window_available_count(&window), 11);


    EXPECT_EQ(sliding_window_active_count(&window), 13 * 4);
    EXPECT_EQ(sliding_window_shrink(&window, -13), 13);
    EXPECT_EQ(sliding_window_shrink(&window, -13), 13);
    EXPECT_EQ(sliding_window_active_count(&window), 13 * 2);

    EXPECT_EQ(sliding_window_data_count(&window), 52);
    EXPECT_EQ(sliding_window_cache_count(&window), 0);
    EXPECT_EQ(sliding_window_active_count(&window), 26);
    EXPECT_EQ(sliding_window_await_count(&window), 26);
    EXPECT_EQ(sliding_window_available_count(&window), 11);

    EXPECT_EQ(sliding_window_shrink(&window, 13), 13);
    EXPECT_EQ(sliding_window_shrink(&window, 13), 13);
    EXPECT_EQ(sliding_window_active_count(&window), 0);

    EXPECT_EQ(sliding_window_data_count(&window), 52);
    EXPECT_EQ(sliding_window_cache_count(&window), 26);
    EXPECT_EQ(sliding_window_active_count(&window), 0);
    EXPECT_EQ(sliding_window_await_count(&window), 26);
    EXPECT_EQ(sliding_window_available_count(&window), 11);

    EXPECT_EQ(sliding_window_slide(&window, -13), 13);
    EXPECT_EQ(sliding_window_slide(&window, -13), 13);

    EXPECT_EQ(sliding_window_data_count(&window), 52);
    EXPECT_EQ(sliding_window_cache_count(&window), 0);
    EXPECT_EQ(sliding_window_active_count(&window), 0);
    EXPECT_EQ(sliding_window_await_count(&window), 52);
    EXPECT_EQ(sliding_window_available_count(&window), 11);

    EXPECT_EQ(sliding_window_dilate(&window, 13), 13);
    EXPECT_EQ(sliding_window_slide(&window, 13), 13);
    EXPECT_EQ(sliding_window_slide(&window, 13), 13);
    EXPECT_EQ(sliding_window_dilate(&window, 13), 13);
    EXPECT_EQ(sliding_window_dilate(&window, 13), 0);

    EXPECT_EQ(sliding_window_data_count(&window), 52);
    EXPECT_EQ(sliding_window_cache_count(&window), 26);
    EXPECT_EQ(sliding_window_active_count(&window), 26);
    EXPECT_EQ(sliding_window_await_count(&window), 0);
    EXPECT_EQ(sliding_window_available_count(&window), 11);


    EXPECT_EQ(sliding_window_slide(&window, -13), 13);
    EXPECT_EQ(sliding_window_slide(&window, -13), 13);
    EXPECT_EQ(sliding_window_slide(&window, 13), 13);
    EXPECT_EQ(sliding_window_slide(&window, 13), 13);

    EXPECT_EQ(sliding_window_data_count(&window), 52);
    EXPECT_EQ(sliding_window_cache_count(&window), 26);
    EXPECT_EQ(sliding_window_active_count(&window), 26);
    EXPECT_EQ(sliding_window_await_count(&window), 0);
    EXPECT_EQ(sliding_window_available_count(&window), 11);
}

TEST(SlidingWindowTest, rx_window_write_test)
{
    sliding_window window;
    sliding_window_init(&window, window_buffer, sizeof(window_buffer), sizeof(uint8_t), false);

    EXPECT_EQ(sliding_window_write(&window, (uint8_t *) "Hello, World!", 13), 0);
    EXPECT_EQ(sliding_window_write(&window, (uint8_t *) "Hello, World!", 13), 0);
    EXPECT_EQ(sliding_window_write(&window, (uint8_t *) "Hello, World!", 13), 0);
    EXPECT_EQ(sliding_window_write(&window, (uint8_t *) "Hello, World!", 13), 0);

    EXPECT_EQ(sliding_window_data_count(&window), 0);
    EXPECT_EQ(sliding_window_cache_count(&window), 0);
    EXPECT_EQ(sliding_window_active_count(&window), 0);
    EXPECT_EQ(sliding_window_await_count(&window), 0);
    EXPECT_EQ(sliding_window_available_count(&window), 63);

    EXPECT_EQ(sliding_window_dilate(&window, 13), 13);
    EXPECT_EQ(sliding_window_dilate(&window, 13), 13);
    EXPECT_EQ(sliding_window_dilate(&window, 13), 13);
    EXPECT_EQ(sliding_window_dilate(&window, 13), 13);

    EXPECT_EQ(sliding_window_data_count(&window), 0);
    EXPECT_EQ(sliding_window_cache_count(&window), 0);
    EXPECT_EQ(sliding_window_active_count(&window), 0);
    EXPECT_EQ(sliding_window_await_count(&window), 52);
    EXPECT_EQ(sliding_window_available_count(&window), 11);

    EXPECT_EQ(sliding_window_write(&window, (uint8_t *) "Hello, World!", 13), 13);
    EXPECT_EQ(sliding_window_write(&window, (uint8_t *) "Hello, World!", 13), 13);

    EXPECT_EQ(sliding_window_data_count(&window), 26);
    EXPECT_EQ(sliding_window_cache_count(&window), 0);
    EXPECT_EQ(sliding_window_active_count(&window), 26);
    EXPECT_EQ(sliding_window_await_count(&window), 26);
    EXPECT_EQ(sliding_window_available_count(&window), 11);

    EXPECT_EQ(sliding_window_write(&window, (uint8_t *) "Hello, World!", 13), 13);
    EXPECT_EQ(sliding_window_write(&window, (uint8_t *) "Hello, World!", 13), 13);
    EXPECT_EQ(sliding_window_data_count(&window), 52);
    EXPECT_EQ(sliding_window_cache_count(&window), 0);
    EXPECT_EQ(sliding_window_active_count(&window), 52);
    EXPECT_EQ(sliding_window_await_count(&window), 0);
    EXPECT_EQ(sliding_window_available_count(&window), 11);

    EXPECT_EQ(sliding_window_slide(&window, 13), 13);
    EXPECT_EQ(sliding_window_slide(&window, 13), 13);
    EXPECT_EQ(sliding_window_data_count(&window), 52);
    EXPECT_EQ(sliding_window_cache_count(&window), 26);
    EXPECT_EQ(sliding_window_active_count(&window), 26);
    EXPECT_EQ(sliding_window_await_count(&window), 0);
    EXPECT_EQ(sliding_window_available_count(&window), 11);
    
    EXPECT_EQ(sliding_window_drop(&window, 13), 13);
    EXPECT_EQ(sliding_window_drop(&window, 13), 13);
    EXPECT_EQ(sliding_window_drop(&window, 13), 0);
    EXPECT_EQ(sliding_window_data_count(&window), 26);
    EXPECT_EQ(sliding_window_cache_count(&window), 0);
    EXPECT_EQ(sliding_window_active_count(&window), 26);
    EXPECT_EQ(sliding_window_await_count(&window), 0);
    EXPECT_EQ(sliding_window_available_count(&window), 37);
    
    EXPECT_EQ(sliding_window_dilate(&window, 13), 13);
    EXPECT_EQ(sliding_window_dilate(&window, 13), 13);
    EXPECT_EQ(sliding_window_data_count(&window), 26);
    EXPECT_EQ(sliding_window_cache_count(&window), 0);
    EXPECT_EQ(sliding_window_active_count(&window),26);
    EXPECT_EQ(sliding_window_await_count(&window), 26);
    EXPECT_EQ(sliding_window_available_count(&window), 11);

    EXPECT_EQ(sliding_window_dilate(&window, 13), 11);
    EXPECT_EQ(sliding_window_write(&window, (uint8_t *) "Hello, World!", 13), 13);
    EXPECT_EQ(sliding_window_slide(&window, 13), 13);
    EXPECT_EQ(sliding_window_drop(&window, 13), 13);

    EXPECT_EQ(sliding_window_data_count(&window), 26);
    EXPECT_EQ(sliding_window_cache_count(&window), 0);
    EXPECT_EQ(sliding_window_active_count(&window),26);
    EXPECT_EQ(sliding_window_await_count(&window), 24);
    EXPECT_EQ(sliding_window_available_count(&window), 13);

    EXPECT_EQ(sliding_window_dilate(&window, 13), 13);
    EXPECT_EQ(sliding_window_write(&window, (uint8_t *) "Hello, World!", 13), 13);
    EXPECT_EQ(sliding_window_slide(&window, 13), 13);
    EXPECT_EQ(sliding_window_drop(&window, 13), 13);

    EXPECT_EQ(sliding_window_data_count(&window), 26);
    EXPECT_EQ(sliding_window_cache_count(&window), 0);
    EXPECT_EQ(sliding_window_active_count(&window),26);
    EXPECT_EQ(sliding_window_await_count(&window), 24);
    EXPECT_EQ(sliding_window_available_count(&window), 13);

    EXPECT_EQ(sliding_window_shrink(&window, -11), 11);
    EXPECT_EQ(sliding_window_write(&window, (uint8_t *) "Hello, World!", 13), 13);
    EXPECT_EQ(sliding_window_dilate(&window, 13), 13);
    EXPECT_EQ(sliding_window_slide(&window, 13), 13);
    EXPECT_EQ(sliding_window_drop(&window, 13), 13);

    EXPECT_EQ(sliding_window_data_count(&window), 26);
    EXPECT_EQ(sliding_window_cache_count(&window), 0);
    EXPECT_EQ(sliding_window_active_count(&window),26);
    EXPECT_EQ(sliding_window_await_count(&window), 13);
    EXPECT_EQ(sliding_window_available_count(&window), 24);
}