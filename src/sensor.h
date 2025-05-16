#ifndef SENSOR_H
#define SENSOR_H

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

struct sensor_data shared_sensor_data;
SemaphoreHandle_t sensor_mutex;

TaskHandle_t heater_task_handle;
TaskHandle_t pump_task_handle;

struct sensor_data
{
    float vgnd;
    float nernst;
    float ipa;
    float lambda;
};

void sensor_gpio_init(void);
void sensor_task(void *parameters);

#endif // SNESOR_H 