#ifndef _SLIDING_WINDOW_H_
#define _SLIDING_WINDOW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct sliding_window_handle 
{
    uint8_t *buffer;
    uint32_t buffer_size;

    uint8_t  elememt_size;

    uint32_t read_index;
    uint32_t write_index;

    uint8_t  front_index;      // front boundary
} sliding_window, *sliding_window_t;

/**
 * @brief Initialize sliding window
 *
 * @param window: sliding window handle
 * @param buffer: buffer to store elements
 * @param buffer_size: buffer size
 * @param element_size: element size
 *
 * @return void
 *
 * @note sliding window is a circular buffer. In current implement, it need an element to distinguish
 *       the front boundary with available write boundary of the sliding window.
 *       Then, the given buffer is recommended to be set actual number required, and plus one.
 *       eg: uint8_t sliding_window_buffer[element_size * element_count + element_size]
 *           then, actual data number is element_count.
 */
void sliding_window_init(sliding_window_t window, void *buffer, uint32_t buffer_size, uint8_t element_size);

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
 * @brief Write element to sliding window
 *
 * @param window: sliding window handle
 * @param element: element to write
 * @param element_count: element count to write
 *
 * @return write element count
 *         -1: error
 */
int32_t sliding_window_write(sliding_window_t window, void *element, uint32_t element_count);

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
int32_t sliding_window_first(sliding_window_t window, void *element);

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
int32_t sliding_window_read(sliding_window_t window, void *element, uint32_t element_count);

/**
 * @brief Drop element from sliding window
 *
 * @param window: sliding window handle
 * @param element_count: element count to drop
 *
 * @return drop element count
 *         -1: error
 */
int32_t sliding_window_drop(sliding_window_t window, uint32_t element_count);

/**
 * @brief Slide window
 *
 * @param window: sliding window handle
 * @param element_count: element count to slide
 *
 * @return slide element count
 *         -1: error
 */
int32_t sliding_window_slide(sliding_window_t window, uint32_t element_count);

#ifdef __cplusplus
}
#endif

#endif // End of _SLIDING_WINDOW_H_
