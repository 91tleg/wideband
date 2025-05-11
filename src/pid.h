#ifndef PID_H
#define PID_H

#include <stdbool.h>

void pid_init(struct pid *data, float kp, float ki, float kd, bool reverse, float out_min, float out_max);

#endif // PID_H