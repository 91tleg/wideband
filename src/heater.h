#ifndef HEATER_H
#define HEATER_H

/*
heater_task: controls temperature based on v_vgnd and v_nernst
*/
extern struct control_loop heater;

void heater_gpio_init(void);
void heater_task(void *parameters);

#endif // HEATER_H