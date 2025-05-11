#ifndef SENSOR_H
#define SNESOR_H


extern QueueHandle_t sensor_queue;

void sensor_task(void *parameters);

#endif // SNESOR_H 