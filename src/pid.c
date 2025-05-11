#include "pid.h"

struct pid 
{
    float *input;
    float *output;
    float *setpoint;

    float kp;
    float ki;
    float kd;
    float last_input;
    float integral;
    float out_min;
    float out_max;

    bool in_auto;
    bool reverse;
};

static void pid_init(struct pid *data, float kp, float ki, float kd, bool reverse, float out_min, float out_max) 
{
    data->kp = reverse ? -kp : kp;
    data->ki = reverse ? -ki : ki;
    data->kd = reverse ? -kd : kd;

    data->reverse = reverse;
    data->integral = 0.0f;
    data->last_input = 0.0f;

    data->out_min = out_min;
    data->out_max = out_max;

    data->in_auto = true;
}