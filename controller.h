#ifndef STD_H
#define STD_H
#include<stdbool.h>
#include<stdint.h>
#include <xc.h>
#endif

#ifndef CONTROLLER_H
#define CONTROLLER_H

#define MIN_INTERVAL_TIME 20
#define MIN_INTERVAL_TIME_MANUAL 10
#define MAX_INTERVAL_TIME 100
#define INTERVAL_CONST 1
#define TURN_OFF_TIME 5000

typedef enum {
    MODE_ANIMATION = 2,
    MODE_BEAT_STROBE = 1,
    MODE_DMX = 0,
    MODE_MANUAL = 3,
    MODE_BEAT_FADE,
    MODE_BEAT_CONTINUOUS,
    MODE_BEAT_MIXED            
}mode_t;


typedef enum {
    CMODE_RED,
    CMODE_GREEN,
    CMODE_BLUE,
    CMODE_WHITE
} colormode_t;

void CONTROLLER_init();
void address_inc();
void address_dec();
void CONTROLLER_task();
uint8_t getAddress();
void menuSelection();
mode_t getMode();
bool getIsHold();

bool static CONTROL_DMX();
bool static CONTROL_BEAT();
bool static CONTROL_MANUAL(colormode_t input);
bool static CONTROL_ANIMATION();

extern bool startup;

#endif