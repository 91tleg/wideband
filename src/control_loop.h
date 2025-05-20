#ifndef CONTROL_LOOP_H
#define CONTROL_LOOP_H

#include "pid.h"

struct control_loop
{
    float input;
    float output;
    float target;
    struct pid pid;
};

void control_loop_init(struct control_loop *loop, float target);

#endif // CONTROL_LOOP_H