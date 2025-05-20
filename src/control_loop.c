#include "control_loop.h"

void control_loop_init(struct control_loop *loop, float target)
{
    loop->input = 0.0f;
    loop->output = 0.0f;
    loop->target = target;
    // PID struct is initiliazed in main with pid_init
}