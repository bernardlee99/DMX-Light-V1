/*
 * File:   led.c
 * Author: student
 *
 * Created on April 26, 2019, 6:34 PM
 */


#include <xc.h>

#include<stdbool.h>
#include "led.h"
#include "beat.h"
#include "clock.h"

time_t lastLedActiveTime = 0;

void LED_init(){
    TRISCbits.TRISC5 = 0;
}

void LED_task(){
    
    if(CLOCK_getTime() - lastLedActiveTime < 100){
        return;
    }
    
    lastLedActiveTime = CLOCK_getTime();
    
    if(BEAT_detected()){
        LATCbits.LATC5 = 1;
    } else {
        LATCbits.LATC5 = 0;
    }
}
