#include "pwm.h"
#include "driver/ledc.h"
#include "defines.h"

#define PWM_FREQ_HZ         (1000U)
#define PWM_RES_BITS        (LEDC_TIMER_10_BIT)
#define PWM_MAX_DUTY        ((1 << PWM_RES_BITS) - 1)

static void pwm_timer_init(void)
{
    ledc_timer_config_t timer = {
        .duty_resolution = PWM_RES_BITS,
        .freq_hz = PWM_FREQ_HZ,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    ledc_timer_config(&timer);
}

static void pwm_heater_init(void)
{
    ledc_channel_config_t heater_channel = {
        .channel    = LEDC_CHANNEL_0,
        .duty       = 0,
        .gpio_num   = PIN_HEATER_PWM,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .hpoint     = 0,
        .timer_sel  = LEDC_TIMER_0,
    };
    ledc_channel_config(&heater_channel);
}

static void pwm_pump_init(void)
{
     ledc_channel_config_t pump_channel = {
        .channel    = LEDC_CHANNEL_1,
        .duty       = 0,
        .gpio_num   = PIN_PUMP_PWM,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .hpoint     = 0,
        .timer_sel  = LEDC_TIMER_0,
    };
    ledc_channel_config(&pump_channel);
}

void pwm_init(void)
{
    pwm_timer_init();
    pwm_heater_init();
    pwm_pump_init();
}

void pwm_set_duty(uint8_t channel, uint32_t duty)
{
    if (duty > PWM_MAX_DUTY) 
    {
        duty = PWM_MAX_DUTY;
    }
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, channel, duty);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, channel);
}
