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

void sliding_window_init(sliding_window_t window, void *buffer, uint32_t buffer_size, uint8_t element_size);

int32_t sliding_window_active_count(sliding_window_t window);

int32_t sliding_window_available_count(sliding_window_t window);

int32_t sliding_window_cache_count(sliding_window_t window);

int32_t sliding_window_write(sliding_window_t window, void *element, uint32_t element_count);

int32_t sliding_window_first(sliding_window_t window, void *element);

int32_t sliding_window_read(sliding_window_t window, void *element, uint32_t element_count);

int32_t sliding_window_drop(sliding_window_t window, uint32_t element_count);

int32_t sliding_window_slide(sliding_window_t window, uint32_t element_count);

#ifdef __cplusplus
}
#endif

#endif // End of _SLIDING_WINDOW_H_
