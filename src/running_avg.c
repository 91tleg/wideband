#include "running_avg.h"

void running_avg_init(struct circular_buffer *cb)
{
    cb->index = 0;
    cb->count = 0;
    cb->sum = 0;
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        cb->buffer[i] = 0;
    }
}

void running_avg_add(struct circular_buffer *cb, float value)
{
    cb->sum -= cb->buffer[cb->index];
    cb->buffer[cb->index] = value;
    cb->sum += value;

    cb->index = (cb->index + 1) % BUFFER_SIZE;
    if (cb->count < BUFFER_SIZE)
    {
        cb->count++;
    }
}

float running_avg_get(const struct circular_buffer *cb)
{
    return (cb->count == 0) ? 0 : cb->sum / cb->count;
}