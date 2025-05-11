#include "sensor.h"
#include "defines.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "running_avg.h"

QueueHandle_t sensor_queue;

struct sensor_data
{
    float v_vgnd;
    float v_nernst;
    float i_pump;
};

static inline float volts(int adc)
{
    return adc / 1024.0 * 5.0;
}

static void gpio_init(void) {
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << PIN_NERNST_PULSE) |
                        (1ULL << PIN_HEATER_PWM) |
                        (1ULL << PIN_PUMP_PWM) |
                        (1ULL << PIN_NARROWBAND_OUT) |
                        (1ULL << PIN_WIDEBAND_OUT),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE
    };
    gpio_config(&io_conf);

    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << PIN_VGND_SENSE) |
                           (1ULL << PIN_NERNST_VOLTAGE) |
                           (1ULL << PIN_PUMP_CURRENT);
    gpio_config(&io_conf);
}

void sensor_task(void *parameters)
{
    gpio_init();
    struct circular_buffer avg_vgnd, avg_nernst, avg_pump;
    running_avg_init(&avg_vgnd);
    running_avg_init(&avg_nernst);
    running_avg_init(&avg_pump);

    while (1)
    {
        int raw_vgnd   = read_adc(PIN_VGND_SENSE);
        int raw_nernst = read_adc(PIN_NERNST_VOLTAGE);
        int raw_pump   = read_adc(PIN_PUMP_CURRENT);

        float v_vgnd   = volts(raw_vgnd);
        float v_nernst = volts(raw_nernst);
        float i_pump   = volts(raw_pump); 

        running_avg_add(&avg_vgnd, v_vgnd);
        running_avg_add(&avg_nernst, v_nernst);
        running_avg_add(&avg_pump, i_pump);

        struct sensor_data data = {
            .v_vgnd = running_avg_get(&avg_vgnd),
            .v_nernst = running_avg_get(&avg_nernst),
            .i_pump = running_avg_get(&avg_pump)
        };

        xQueueSend(sensor_queue, &data, portMAX_DELAY);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
