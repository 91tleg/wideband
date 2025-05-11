#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "sensor.h"
#include "running_avg.h"
/*
[ADC Inputs] --> [Sensor Read Task] --> [Circular Buffers] --> [PID Inputs] --> [Heat / Pump Tasks]
                                                    \
                                                    --> [Lambda Conversion Task] --> [Lambda Output (PWM/DAC)]
*/

void app_main(void) 
{
    
}
