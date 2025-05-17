#ifndef PWM_H
#define PWM_H

#include <stdint.h>

void pwm_init(void);
void pwm_set_duty(uint8_t channel, uint32_t duty);

#endif // PWM_H