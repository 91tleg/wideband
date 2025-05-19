#ifndef PID_H
#define PID_H

#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    PID_OK = 1,
    PID_SKIPPED = 0,
    PID_DISABLED = -1
} pid_status_e;

typedef enum
{
    PID_DIRECT = 1,
    PID_REVERSE = -1
} pid_direction_e;

typedef enum
{
    P_ON_MEASUREMENT = 0,
    P_ON_ERROR = 1
} pid_proportional_mode_e;

typedef enum 
{
    PID_MODE_MANUAL = 0,
    PID_MODE_AUTOMATIC = 1
} pid_mode_e;


struct pid
{
    float *input;
    float *output;
    float *setpoint;

    float kp;    // Proportional gain
    float ki;    // Integral gain
    float kd;    // Derivative gain

    float last_input;
    float integral;

    float out_min;
    float out_max;

    int64_t sample_time;
    int64_t last_time;

    pid_direction_e controller_direction;
    pid_proportional_mode_e p_on;
    pid_mode_e mode;
};

void pid_init(struct pid *data, float *input, float *output, float *setpoint,
              float kp, float ki, float kd, float sample_time_ms, float out_min, 
              float out_max, pid_direction_e direction, pid_mode_e mode);
void pid_set_tunings(struct pid *data, float kp, float ki, float kd);
void pid_set_output_limits(struct pid *data, float min, float max);
void pid_set_sample_time(struct pid *data, int64_t sample_time_ms);
void pid_set_direction(struct pid *data, pid_direction_e direction);
void pid_set_mode(struct pid *data, pid_mode_e mode);
pid_status_e pid_compute(struct pid *data);

#endif // PID_H