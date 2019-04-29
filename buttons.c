/*
 * File:   buttons.c
 * Author: berna
 *
 * Created on April 14, 2019, 1:39 PM
 */


#include "clock.h"
#include "buttons.h"

int lastTime = 0;
button_t buttons[BUTTON_COUNT];
extern button_t *up, *down, *menu, *enter;

static void init(button_t* btn, volatile uint8_t* port, uint8_t pin) {
    btn->port = port;
    btn->pin = pin;
    btn->state = STATE_UNPRESSED;
    btn->event = EVENT_IDLE;
}

void BUTTONS_init() {
    menu = &buttons[0];
    up = &buttons[1];
    down = &buttons[2];
    enter = &buttons[3];

    init(menu, &PORTC, 2);
    init(enter, &PORTA, 2);
    init(down, &PORTC, 0);
    init(up, &PORTC, 1);
    
    GPIO_init();
    
}

void static GPIO_init(){
    
    TRISAbits.TRISA2 = 1;
    ANSELAbits.ANSA2 = 0;
    WPUAbits.WPUA2 = 1;
    
    TRISCbits.TRISC0 = 1;
    TRISCbits.TRISC1 = 1;
    TRISCbits.TRISC2 = 1;
    ANSELCbits.ANSC0 = 0;
    ANSELCbits.ANSC1 = 0;
    ANSELCbits.ANSC2 = 0;
    WPUCbits.WPUC0 = 1;
    WPUCbits.WPUC1 = 1;
    WPUCbits.WPUC2 = 1;
        
}

void BUTTONS_task() {
    
    time_t time = CLOCK_getTime();

    // only run every 10 ms
    if (time - lastTime < BUTTON_SCAN_INTERVAL)
        return;

    lastTime = time;

    // walk through the list of buttons and update everything
    for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
        button_t* btn = buttons + i;
        
        // figure out if the button is currently activated or not
        if ((*(btn->port) & (1 << (btn->pin))) == 0) {
            btn->state = STATE_PRESSED;
            if(btn->event == EVENT_IDLE){
                btn->lastHoldTime = time;
            }
        } else {
            btn->state = STATE_UNPRESSED;
        }

        if (btn->state == STATE_UNPRESSED && btn->lastState == STATE_PRESSED) {
            // we released the button
            btn->event = EVENT_PRESSED;
        } else if (btn->state == STATE_PRESSED && btn->lastState == STATE_PRESSED){
            if(time - btn->lastHoldTime > HOLD_TIME){
                btn->event = EVENT_HELD;
            } else if(btn->event != EVENT_HELD) {
                btn->event = EVENT_INTER_HOLD;
            }
        } else if(btn->state == STATE_UNPRESSED && btn->lastState == STATE_PRESSED && btn->event == EVENT_HELD){
            btn->event = EVENT_IDLE;
        }

        btn->lastState = btn->state;
    }
}

bool BUTTONS_isClicked(button_t* button) {
    if (button->event == EVENT_PRESSED) {
        button->event = EVENT_IDLE;
        return true;
    }
    
    return false;
}

bool BUTTONS_isHeld(button_t* button) {
    if(button->event == EVENT_HELD){
        return true;
    }
    
    return false;
    
}