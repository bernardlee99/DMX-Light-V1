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

void CONTROLLER_init();

typedef enum {
    DMX,
    BEAT,
    ANIMATION,
    MANUAL,
    DMX_TASK,
    B_STROBE,
    B_FADE,
    B_CONT,
    B_MIXED,
    B_STROBE_TASK,
    B_FADE_TASK,
    B_CONT_TASK,
    B_MIXED_TASK,
    MANUAL_RED,
    MANUAL_GREEN,
    MANUAL_BLUE,
    MANUAL_WHITE,
    A_BRIGHTNESS,
    A_SELECTION,
    A_SPEED
}menu_t;

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

void static CONTROL_DMX();
void static CONTROL_BEAT();
void static CONTROL_MANUAL();
void static CONTROL_ANIMATION();

menu_t getCurrentMenu();

extern bool startup;

#endif