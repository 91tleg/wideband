#ifndef PUMP_H
#define PUMP_H

/*
pump_task: controls air-fuel measurement based on v_nernst and i_pump
*/

extern struct control_loop pump;

void pump_gpio_init(void);
void pump_task(void *parameters);

#endif // PUMP_H