#include "heater.h"
#include "pid.h"
#include "defines.h"
#include "sensor.h"
#include "pwm.h"
#include "control_loop.h"

#define HEATER_CHANNEL (0)

struct control_loop heater;

void heater_gpio_init(void)
{
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << PIN_HEATER_PWM),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE};
    gpio_config(&io_conf);
}

void heater_task(void *parameters)
{
    struct sensor_data data;

    while (1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        if (xSemaphoreTake(sensor_mutex, pdMS_TO_TICKS(10)) == pdTRUE)
        {
            data = shared_sensor_data;
            xSemaphoreGive(sensor_mutex);
        }

        while (heater.output < INITIAL_RAMPUP_TILL)
        {
            heater.input = data.nernst_r;
            if (heater.input < HEAT_TARGET)
            {
                break;
            }
            heater.output += INITIAL_RAMPUP_STEP;
            pwm_set_duty(HEATER_CHANNEL, heater.output);
            vTaskDelay(pdMS_TO_TICKS(100));
        }

        if (pid_compute(&heater.pid) == PID_OK)
        {
            pwm_set_duty(HEATER_CHANNEL, heater.output);
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}