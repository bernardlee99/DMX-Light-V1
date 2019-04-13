/*
 * File:   button.c
 * Author: berna
 *
 * Created on April 13, 2019, 10:59 AM
 */


#include <xc.h>

//void BUTTONS_task() {
//    time_t time = CLOCK_getTime();
//
//    // only run every 10 ms
//    if (time - lastTime < BUTTON_SCAN_INTERVAL)
//        return;
//
//    lastTime = time;
//
//    // walk through the list of buttons and update everything
//    for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
//        button_t* btn = buttons + i;
//        
//        // figure out if the button is currently activated or not
//        if ((*(btn->port) & (1 << (btn->pin))) == 0) {
//            btn->state = STATE_PRESSED;
//        } else {
//            btn->state = STATE_UNPRESSED;
//        }
//
//        if (
//            btn->state == STATE_UNPRESSED 
//            && btn->lastState == STATE_PRESSED
//           ) {
//            // we released the button
//            btn->event = EVENT_PRESSED;
//        }
//
//        btn->lastState = btn->state;
//    }
//}