#ifndef STD_H
#define STD_H
#include<stdbool.h>
#include<stdint.h>
#include <xc.h>
#endif

#ifndef BEAT_H
#define BEAT_H

static void adcThresholdHandler();
void BEAT_init();
bool BEAT_detected();
void BEAT_task();

extern uint8_t beatBrightness;
extern uint8_t animationBrightness;

#endif