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
#include "mcc_generated_files/mcc.h"

time_t lastLedActiveTime = 0;
int currentState = 0;

void LED_init(){
    TRISCbits.TRISC5 = 0;
}

void LED_task(){
    
    if(CLOCK_getTime() - lastLedActiveTime < 100){
        return;
    }
    
    lastLedActiveTime = CLOCK_getTime();
        
    if(!BEAT_detected()){
        PWM1_LoadDutyValue(0);
        PWM2_LoadDutyValue(0);
        PWM3_LoadDutyValue(0);
        PWM4_LoadDutyValue(0);
    } else if (currentState == 1){
        PWM1_LoadDutyValue(120);
        PWM2_LoadDutyValue(0);
        PWM3_LoadDutyValue(0);
        PWM4_LoadDutyValue(0);
        currentState++;
    } else if(currentState == 2){
        PWM1_LoadDutyValue(0);
        PWM2_LoadDutyValue(120);
        PWM3_LoadDutyValue(120);
        PWM4_LoadDutyValue(0);
        currentState++;
    } else if(currentState == 3){
        PWM1_LoadDutyValue(120);
        PWM2_LoadDutyValue(0);
        PWM3_LoadDutyValue(120);
        PWM4_LoadDutyValue(0);
        currentState++;
    } else if(currentState == 4){
        PWM1_LoadDutyValue(0);
        PWM2_LoadDutyValue(0);
        PWM3_LoadDutyValue(120);
        PWM4_LoadDutyValue(120);
        currentState++;
    }else if(currentState == 5){
        PWM1_LoadDutyValue(120);
        PWM2_LoadDutyValue(120);
        PWM3_LoadDutyValue(120);
        PWM4_LoadDutyValue(0);
        currentState++;
    } else if(currentState == 6){
        PWM1_LoadDutyValue(120);
        PWM2_LoadDutyValue(0);
        PWM3_LoadDutyValue(120);
        PWM4_LoadDutyValue(120);
        currentState++;
    } else if(currentState == 8){
        PWM1_LoadDutyValue(0);
        PWM2_LoadDutyValue(120);
        PWM3_LoadDutyValue(120);
        PWM4_LoadDutyValue(120);
        currentState++;
    } else {
        PWM1_LoadDutyValue(120);
        PWM2_LoadDutyValue(120);
        PWM3_LoadDutyValue(120);
        PWM4_LoadDutyValue(120);
        currentState = 1;
    }
    
}
