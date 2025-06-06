#pragma once

#include "driver/gpio.h"

#define TARGET_LAMBDA (1.0f)
#define WIDEBAND_TOP (2.0f)
#define WARMUP_DELAY (20000U)
#define INITIAL_RAMPUP_TILL (1023.0f / 14.0f * 7.5f)
#define INITIAL_RAMPUP_STEP (1023.0f / (14.0f / 0.5f))

#define HEAT_TARGET (2.3f / 1300.0f * 200.0f)
#define HEAT_SAMPLE_TIME (500)
#define PUMP_TARGET (0.45f)
#define PUMP_SAMPLE_TIME (40)

// In
#define PIN_VGND_SENSE        (GPIO_NUM_34) 
#define PIN_NERNST_VOLTAGE    (GPIO_NUM_35)
#define PIN_PUMP_CURRENT      (GPIO_NUM_32)

// Out
#define PIN_NERNST_PULSE      (GPIO_NUM_25)
#define PIN_NARROWBAND_OUT    (GPIO_NUM_14)
#define PIN_WIDEBAND_OUT      (GPIO_NUM_12)

// PWM
#define PIN_HEATER_PWM      (GPIO_NUM_26)
#define PIN_PUMP_PWM        (GPIO_NUM_27)