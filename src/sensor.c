#include "sensor.h"
#include "defines.h"
#include "esp_log.h"
#include "lambda.h"

struct sensor_avg avg;
struct sensor_data shared_sensor_data;
SemaphoreHandle_t sensor_mutex;
TaskHandle_t heater_task_handle;
TaskHandle_t pump_task_handle;


static inline float volts(int adc)
{
    return adc / 1024.0f * 5.0f;
}

static float vgnd_volts(void)
{
    running_avg_add(&avg.vgnd, volts(gpio_get_level(PIN_VGND_SENSE)));
    return running_avg_get(&avg.vgnd);
}

static float nernst_volts(void)
{
    running_avg_add(&avg.nernst, volts(gpio_get_level(PIN_NERNST_PULSE)));
    return running_avg_get(&avg.nernst);
}

static float pump_volts(void)
{
    running_avg_add(&avg.ipa, volts(gpio_get_level(PIN_NERNST_PULSE)));
    return running_avg_get(&avg.ipa);
}

static float nernst_resistance_volts(void)
{
    int before = gpio_get_level(PIN_NERNST_VOLTAGE);
    gpio_set_level(PIN_NERNST_PULSE, 1);
    vTaskDelay(pdMS_TO_TICKS(30));

    int after = gpio_get_level(PIN_NERNST_VOLTAGE);
    gpio_set_level(PIN_NERNST_PULSE, 0);
    vTaskDelay(pdMS_TO_TICKS(200));

    return volts(after - before);
}

static void input_gpio_init(void)
{
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << PIN_VGND_SENSE) |
                        (1ULL << PIN_NERNST_VOLTAGE) |
                        (1ULL << PIN_PUMP_CURRENT),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE};
    gpio_config(&io_conf);
}

static void output_gpio_init(void)
{
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << PIN_NERNST_PULSE) |
                        (1ULL << PIN_NARROWBAND_OUT) |
                        (1ULL << PIN_WIDEBAND_OUT),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE};
    gpio_config(&io_conf);
}

void sensor_gpio_init(void)
{
    input_gpio_init();
    output_gpio_init();
}

void sensor_task(void *parameters)
{
    while (1)
    {
        float vgnd = vgnd_volts();
        float nernst = nernst_volts();
        float ipa = pump_volts();
        float nernst_r = nernst_resistance_volts();
        float lambda = get_lambda(ipa);

        struct sensor_data data = {
            .vgnd = vgnd,
            .nernst = nernst,
            .ipa = ipa,
            .nernst_r = nernst_r,
            .lambda = lambda};

        ESP_LOGI("SENSOR", "vgnd:%f  nernst:%f  pump:%f  lambda:%f",
                 data.vgnd, data.nernst, data.ipa, data.lambda);

        if (xSemaphoreTake(sensor_mutex, pdMS_TO_TICKS(10)) == pdTRUE)
        {
            shared_sensor_data = data;
            xSemaphoreGive(sensor_mutex);
        }

        xTaskNotifyGive(heater_task_handle);
        xTaskNotifyGive(pump_task_handle);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
