#ifndef _SLIDING_WINDOW_H_
#define _SLIDING_WINDOW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef struct sliding_window_handle 
{
    uint8_t *buffer;
    uint16_t buffer_size;

    uint8_t  elememt_size;
    uint8_t  is_tx_window;

    uint16_t read_index;
    uint16_t write_index;

    uint16_t front_index;  // front boundary
    uint16_t back_index;   // back boundary
} sliding_window, *sliding_window_t;

/**
 * @brief Initialize sliding window
 *
 * @param window: sliding window handle
 * @param buffer: buffer to store elements
 * @param buffer_size: buffer size
 * @param element_size: element size
 * @param is_tx_window: if true, sliding window is tx mode, otherwise, rx mode
 *
 * @return void
 *
 * @note sliding window is a circular buffer. In current implement, it need an element to distinguish
 *       the front boundary with available write boundary of the sliding window.
 *       Then, the given buffer is recommended to be set actual number required, and plus one.
 *       eg: uint8_t sliding_window_buffer[element_size * element_count + element_size]
 *           then, actual data number is element_count.
 */
void sliding_window_init(sliding_window_t window, void *buffer, uint16_t buffer_size, uint8_t element_size, bool is_tx_window);


int32_t sliding_window_data_count(sliding_window_t window);

/**
 * @brief Get active(readable) element count
 *
 * @param window: sliding window handle
 *
 * @return active element count
 *         -1: error
 */
int32_t sliding_window_active_count(sliding_window_t window);

/**
 * @brief Get available(writeable) element count
 *
 * @param window: sliding window handle
 *
 * @return available element count
 *         -1: error
 */
int32_t sliding_window_available_count(sliding_window_t window);


/**
 * @brief Get cache(waiting drop) element count
 *
 * @param window: sliding window handle
 *
 * @return cache element count
 *         -1: error
 */
int32_t sliding_window_cache_count(sliding_window_t window);

/**
 * @brief Get await(waiting read but current not readable) data count
 *
 * @param window: sliding window handle
 *
 * @return await element count
 *         -1: error
 */
int32_t sliding_window_await_count(sliding_window_t window);


int32_t sliding_window_valid_count(sliding_window_t window);

/**
 * @brief Write element to sliding window
 *
 * @param window: sliding window handle
 * @param element: element to write
 * @param element_count: element count to write
 *
 * @return write element count
 *         -1: error
 */
int32_t sliding_window_write(sliding_window_t window, void *element, uint16_t element_count);

/**
 * @brief Read element from sliding window
 *
 * @param window: sliding window handle
 * @param element: element to read
 * @param element_count: element count to read
 *
 * @return read element count
 *         -1: error
 */
int32_t sliding_window_read(sliding_window_t window, void *element, uint16_t element_count);

/**
 * @brief Drop element from sliding window
 *
 * @param window: sliding window handle
 * @param element_count: element count to drop
 *
 * @return drop element count
 *         -1: error
 */
int32_t sliding_window_drop(sliding_window_t window, uint16_t element_count);

/**
 * @brief Slide window forward
 *
 * @param window: sliding window handle
 * @param element_count: element count to slide
 *                       possitive: slide forward
 *                       negative: slide backward
 *
 * @return slide element count
 *         -1: error
 */
int32_t sliding_window_slide(sliding_window_t window, int32_t element_count);

/**
 * @brief Slide window shrink
 *
 * @param window: sliding window handle
 * @param element_count: element count to shrink
 *                       possitive: forward shrink
 *                       negative: backward shrink
 *
 * @return slide element count
 *         -1: error
 */
int32_t sliding_window_shrink(sliding_window_t window, int32_t element_count);

/**
 * @brief Slide window dilate
 *
 * @param window: sliding window handle
 * @param element_count: element count to dilate
 *                       negative: forward dilate
 *                       possitive: backward dilate
 */
int32_t sliding_window_dilate(sliding_window_t window, int32_t element_count);

/**
 * @brief Reset sliding window active window size
 * 
 * @param window: sliding window handle
 * @param element_count: element count to reset
 */
int32_t sliding_window_active_reset(sliding_window_t window, uint16_t element_count);


int32_t sliding_window_valid_reset(sliding_window_t window, uint16_t element_count);


#ifdef __cplusplus
}
#endif

#endif // End of _SLIDING_WINDOW_H_
