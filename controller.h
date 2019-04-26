#ifndef CONTROLLER_H
#define CONTROLLER_H

#define MIN_INTERVAL_TIME 0
#define MAX_INTERVAL_TIME 100
#define INTERVAL_CONST 1
#define TURN_OFF_TIME 5000

void CONTROLLER_init();
void address_inc();
void address_dec();
void CONTROLLER_task();


#endif