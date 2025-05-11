#ifndef RUNNING_AVG_H
#define RUNNING_AVG_H

#define BUFFER_SIZE 10
struct circular_buffer
{
    float buffer[BUFFER_SIZE];
    int index;
    int count;
    float sum;
};

void running_avg_init(struct circular_buffer *cb);
void running_avg_add(struct circular_buffer *cb, float value);
float running_avg_get(const struct circular_buffer *cb);

#endif // RUNNING_AVG_H