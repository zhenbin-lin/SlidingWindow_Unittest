#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "sliding_window.h"

#define SLIDING_WINDOW_MIN(lhs, rhs)     ((lhs) < (rhs) ? (lhs) : (rhs))
#define SLIDING_WINDOW_MAX(lhs, rhs)     ((lhs) > (rhs) ? (lhs) : (rhs))

void sliding_window_init(sliding_window_t window, void *buffer, uint16_t buffer_size, uint8_t element_size)
{
    if (window == NULL || buffer == NULL)
        return;

    window->buffer = (uint8_t*)buffer;
    window->buffer_size = buffer_size;
    window->elememt_size = element_size;

    window->read_index = 0;
    window->write_index = 0;
    window->front_index = 0;
}

int32_t sliding_window_active_count(sliding_window_t window)
{
    if (window == NULL)
        return -1;

    if (window->read_index <= window->write_index)
    {
        return (window->write_index - window->read_index) / window->elememt_size;
    }
    else 
    {
        return (window->buffer_size - window->read_index + window->write_index) / window->elememt_size;
    }
}

int32_t sliding_window_available_count(sliding_window_t window)
{
    if (window == NULL)
        return -1;

    if (window->write_index + window->elememt_size <= window->front_index)
    {
        return (window->front_index - window->write_index) / window->elememt_size - 1;
    }
    else 
    {
        return (window->buffer_size - window->write_index + window->front_index) / window->elememt_size - 1;
    }
}

int32_t sliding_window_cache_count(sliding_window_t window)
{
    if (window == NULL)
        return -1;

    if (window->front_index <= window->read_index)
    {
        return (window->read_index - window->front_index) / window->elememt_size;
    }
    else
    {
        return (window->buffer_size - window->front_index + window->read_index) / window->elememt_size;
    }
}

int32_t sliding_window_write(sliding_window_t window, void *element, uint16_t element_count)
{
    uint16_t ready_to_write; // number of elements ready to be written
    uint16_t available_count;

    if (window == NULL || element == NULL)
        return -1;

    if (element_count == 0)
        return 0;

    available_count = sliding_window_available_count(window);
    ready_to_write = SLIDING_WINDOW_MIN(available_count, element_count);

    if (ready_to_write == 0)
        return 0;

    if (window->write_index + window->elememt_size <= window->front_index)
    {
        memcpy(window->buffer + window->write_index, element, ready_to_write * window->elememt_size);
        window->write_index += ready_to_write * window->elememt_size;
    }
    else
    {
        if (window->write_index + ready_to_write * window->elememt_size <= window->buffer_size)
        {
            memcpy(window->buffer + window->write_index, element, ready_to_write * window->elememt_size);
            window->write_index += ready_to_write * window->elememt_size;
        }
        else
        {
            memcpy(window->buffer + window->write_index, element, (window->buffer_size - window->write_index) * window->elememt_size);
            memcpy(window->buffer, (uint8_t*)element + ((window->buffer_size - window->write_index) * window->elememt_size), (ready_to_write - (window->buffer_size - window->write_index)) * window->elememt_size);
            window->write_index = (window->write_index + ready_to_write * window->elememt_size) % window->buffer_size;
        }
    }
    
    return ready_to_write;
}

int32_t sliding_window_read(sliding_window_t window, void *element, uint16_t element_count)
{
    uint16_t ready_to_read; // number of elements ready to be read
    uint16_t active_count;

    if (window == NULL || element == NULL)
        return -1;

    if (element_count == 0)
        return 0;

    active_count = sliding_window_active_count(window);
    ready_to_read = SLIDING_WINDOW_MIN(active_count, element_count);

    if (ready_to_read == 0)
        return 0;

    if (window->read_index < window->write_index)
    {
        memcpy(element, window->buffer + window->read_index, ready_to_read * window->elememt_size);
    }
    else 
    {
        if (window->read_index + ready_to_read * window->elememt_size <= window->buffer_size)
        {
            memcpy(element, window->buffer + window->read_index, ready_to_read * window->elememt_size);
        }
        else 
        {
            memcpy(element, window->buffer + window->read_index, window->buffer_size - window->read_index);
            memcpy((uint8_t*)element + (window->buffer_size - window->read_index), window->buffer, ready_to_read * window->elememt_size - (window->buffer_size - window->read_index));
        }
    }

    return ready_to_read;
}

int32_t sliding_window_drop(sliding_window_t window, uint16_t element_count)
{
    uint16_t cache_count;
    uint16_t ready_to_drop;

    if (window == NULL)
        return -1;

    if (element_count == 0)
        return 0;

    cache_count = sliding_window_cache_count(window);
    ready_to_drop = SLIDING_WINDOW_MIN(cache_count, element_count);

    if (ready_to_drop == 0)
        return 0;

    window->front_index += ready_to_drop * window->elememt_size;

    return ready_to_drop;
}


int32_t sliding_window_slide(sliding_window_t window, uint16_t element_count)
{
    uint16_t active_count;
    uint16_t ready_to_slide;

    if (window == NULL)
        return -1;

    if (element_count == 0)
        return 0;

    active_count = sliding_window_active_count(window);
    ready_to_slide = SLIDING_WINDOW_MIN(active_count, element_count);

    if (ready_to_slide == 0)
        return 0;

    window->read_index += ready_to_slide * window->elememt_size;

    return ready_to_slide;
}

int32_t sliding_window_slide_back(sliding_window_t window, uint16_t element_count)
{
    uint16_t cache_count;
    uint16_t ready_to_slide;

    if (window == NULL)
        return -1;

    if (element_count == 0)
        return 0;

    cache_count = sliding_window_cache_count(window);
    ready_to_slide = SLIDING_WINDOW_MIN(cache_count, element_count);

    if (ready_to_slide == 0)
        return 0;

    window->read_index = ((int32_t)window->read_index - ready_to_slide * window->elememt_size + window->buffer_size) % window->buffer_size;

    return ready_to_slide;
}

