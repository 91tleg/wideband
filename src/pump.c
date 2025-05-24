#include "pump.h"
#include "pid.h"
#include "defines.h"
#include "sensor.h"
#include "pwm.h"
#include "control_loop.h"

#define PUMP_CHANNEL (1)

struct control_loop pump;

void pump_gpio_init(void)
{
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << PIN_PUMP_PWM),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE};
    gpio_config(&io_conf);
}

void pump_task(void *parameters)
{
    struct sensor_data data;

    while (1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        if ((xSemaphoreTake(sensor_mutex, pdMS_TO_TICKS(10)) == pdTRUE))
        {
            data = shared_sensor_data;
            xSemaphoreGive(sensor_mutex);
        }

        pump.input = data.nernst - data.vgnd;
        if (pid_compute(&pump.pid) == PID_OK)
        {
            pwm_set_duty(PUMP_CHANNEL, pump.output);
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}