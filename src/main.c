#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "sensor.h"
#include "pump.h"
#include "heater.h"
#include "pwm.h"
#include "pid.h"
#include "defines.h"
#include "control_loop.h"
#include "running_avg.h"

void app_main(void)
{
    sensor_gpio_init();
    pump_gpio_init();
    heater_gpio_init();
    pwm_init();
    control_loop_init(&heater, HEAT_TARGET);
    control_loop_init(&pump, 1023.0f);
    pid_init(
        &pump.pid, &pump.input, &pump.output, &pump.target, 
        800.0f, 100.0f, 0.001f, PUMP_SAMPLE_TIME, 0.0f, 
        1023.0f, PID_DIRECT, PID_MODE_AUTOMATIC);
    pid_init(
        &heater.pid, &heater.input, &heater.output, &heater.target, 
        800.0f, 100.0f, 1.0f, HEAT_SAMPLE_TIME, 0.0f, 
        INITIAL_RAMPUP_TILL, PID_REVERSE, PID_MODE_AUTOMATIC);
    running_avg_init(&avg.vgnd);
    running_avg_init(&avg.ipa);
    running_avg_init(&avg.nernst);

    sensor_mutex = xSemaphoreCreateMutex();

    xTaskCreate(
        sensor_task,
        "SensorTask",
        2048,
        NULL,
        3,
        NULL);

    xTaskCreate(
        heater_task,
        "HeaterTask",
        2048,
        NULL,
        3,
        &heater_task_handle);

    xTaskCreate(
        pump_task,
        "PumpTask",
        2048,
        NULL,
        3,
        &pump_task_handle);
}
