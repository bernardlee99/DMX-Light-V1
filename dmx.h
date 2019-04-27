#ifndef STD_H
#define STD_H
#include<stdbool.h>
#include<stdint.h>
#include <xc.h>
#endif

#ifndef DMX_H

#define DMX_H

void DMX_init();
void address_inc();
void address_dec();
uint8_t getAddress();
void DMX_task();

extern uint16_t address;

#endif