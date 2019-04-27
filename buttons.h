#ifndef STD_H
#define STD_H
#include<stdbool.h>
#include<stdint.h>
#include <xc.h>
#endif

#ifndef BUTTON_H
#define BUTTON_H
typedef enum {
    EVENT_IDLE,
    EVENT_PRESSED,
    EVENT_INTER_HOLD,
    EVENT_HELD
} event_t;

typedef enum {
    STATE_UNPRESSED,
    STATE_PRESSED
} btnState_t;

typedef struct {
    volatile uint8_t* port;
    uint8_t pin;
    btnState_t state;
    btnState_t lastState;
    time_t lastHoldTime;
    event_t event;
} button_t;

#define BUTTON_SCAN_INTERVAL 2
#define BUTTON_COUNT 4
#define HOLD_TIME 500

void BUTTONS_task();
void BUTTONS_init();
static void init(button_t* btn, volatile uint8_t* port, uint8_t pin);
void static GPIO_init();
bool BUTTONS_isClicked(button_t* button);
bool BUTTONS_isHeld(button_t* button);

#endif