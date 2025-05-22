#include "pid.h"
#include "esp_timer.h"
#include <string.h>

void pid_init(struct pid *data, float *input, float *output, float *setpoint,
              float kp, float ki, float kd, float sample_time_ms, float out_min,
              float out_max, pid_direction_e direction, pid_mode_e mode)
{
    data->input = input;
    data->output = output;
    data->setpoint = setpoint;

    data->p_on = P_ON_ERROR;

    data->last_input = *input;
    data->integral = 0.0f;
    data->last_time = esp_timer_get_time() / 1000; // ms

    pid_set_sample_time(data, sample_time_ms);
    pid_set_tunings(data, kp, ki, kd);
    pid_set_output_limits(data, out_min, out_max);
    pid_set_direction(data, direction);
    pid_set_mode(data, PID_MODE_AUTOMATIC);
}

static inline float clamp(float value, float min, float max)
{
    return (value > max) ? max : (value < min) ? min : value;
}

static inline void pid_negate_gains(struct pid *data)
{
    data->kp = -data->kp;
    data->ki = -data->ki;
    data->kd = -data->kd;
}

void pid_set_tunings(struct pid *data, float kp, float ki, float kd)
{
    int64_t sample_time_sec = data->sample_time / 1000;
    data->kp = kp;
    data->ki = ki * (float)sample_time_sec;
    data->kd = kd / (float)sample_time_sec;
}

void pid_set_output_limits(struct pid *data, float min, float max)
{
    if (min >= max)
    {
        return;
    }
    data->out_min = min;
    data->out_max = max;
    data->integral = clamp(data->integral, min, max);
    *data->output = clamp(*data->output, min, max);
}

void pid_set_sample_time(struct pid *data, int64_t sample_time_ms)
{
    if (sample_time_ms > 0)
    {
        float ratio = (float)sample_time_ms / (float)data->sample_time;

        data->ki *= ratio;
        data->kd /= ratio;
        data->sample_time = sample_time_ms;
    }
}

void pid_set_direction(struct pid *data, pid_direction_e direction)
{
    if (direction != data->controller_direction)
    {
        pid_negate_gains(data);
        data->controller_direction = direction;
    }
}

void pid_set_mode(struct pid *data, pid_mode_e mode)
{
    if (mode == PID_MODE_AUTOMATIC && data->mode != PID_MODE_AUTOMATIC)
    {
        data->last_input = *data->input;
        data->integral = *data->output;
        data->integral = clamp(data->integral, data->out_min, data->out_max);
    }
    data->mode = mode;
}

void pid_reset(struct pid *data)
{
    memset(data, 0, sizeof(*data));
    pid_set_direction(data, PID_DIRECT);
    pid_set_sample_time(data, 100);
    pid_set_output_limits(data, 0.0f, 1023.0f);
}

pid_status_e pid_compute(struct pid *data)
{
    if (data->mode != PID_MODE_AUTOMATIC)
    {
        return PID_DISABLED;
    }

    int64_t now = esp_timer_get_time() / 1000;
    int64_t time_change = now - data->last_time;

    if (time_change >= data->sample_time)
    {
        float input = *data->input;
        float error = *data->setpoint - input;
        float d_input = input - data->last_input;

        data->integral += data->ki * error;
        data->integral = clamp(data->integral, data->out_min, data->out_max);

        float output = (data->p_on == P_ON_ERROR) ? (data->kp * error) : (-data->kp * d_input);

        output += data->integral - data->kd * d_input;
        *data->output = clamp(output, data->out_min, data->out_max);

        data->last_input = input;
        data->last_time = now;
        return PID_OK;
    }
    return PID_SKIPPED;
}