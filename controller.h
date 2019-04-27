#ifndef STD_H
#define STD_H
#include<stdbool.h>
#include<stdint.h>
#include <xc.h>
#endif

#ifndef CONTROLLER_H
#define CONTROLLER_H

#define MIN_INTERVAL_TIME 20
#define MAX_INTERVAL_TIME 100
#define INTERVAL_CONST 1
#define TURN_OFF_TIME 5000

typedef enum {
    MODE_ANIMATION,
    MODE_BEAT,
    MODE_DMX    
}mode_t;

void CONTROLLER_init();
void address_inc();
void address_dec();
void CONTROLLER_task();
uint8_t getAddress();
void menuSelection();

bool static CONTROL_DMX();

#endif