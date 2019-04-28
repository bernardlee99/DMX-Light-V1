/*
 * File:   led.c
 * Author: student
 *
 * Created on April 26, 2019, 6:34 PM
 */


#include "led.h"
#include "beat.h"
#include "clock.h"
#include "controller.h"
#include "mcc_generated_files/mcc.h"
#include "dmx.h"

time_t lastLedActiveTime = 0;
int currentState = 0;
uint8_t beatBrightness = 1;
 int dmxArray[513];
 bool startup;

void LED_setColor(color_t input){
    if(!startup){
        PWM1_LoadDutyValue( ( ((int)input.red)/255.0 ) * 120 );
        PWM2_LoadDutyValue( ( ((int)input.green)/255.0 ) * 120 );
        PWM3_LoadDutyValue( ( ((int)input.blue)/255.0 ) * 120 );
        PWM4_LoadDutyValue( ( ((int)input.white)/255.0 ) * 120 );
    }
}

void static LED_task_DMX(){
    
    color_t dmx;

    dmx.red = dmxArray[DMX_getAddress()];
    dmx.green = dmxArray[DMX_getAddress() + 1];
    dmx.blue = dmxArray[DMX_getAddress() + 2];
    dmx.white = dmxArray[DMX_getAddress() + 3];
    
    LED_setColor(dmx);

}

void LED_task(){
    
    switch(getMode()){
        case MODE_DMX:
            LED_task_DMX();
            break;
            
        case MODE_BEAT:
            LED_task_BEAT();
            break;
            
        case MODE_ANIMATION:
            LED_task_ANIMATION();
            break;
    }
    
}

void static LED_task_BEAT(){
    
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
        PWM1_LoadDutyValue((float)(beatBrightness/9.0) * 120);
        PWM2_LoadDutyValue(0);
        PWM3_LoadDutyValue(0);
        PWM4_LoadDutyValue(0);
        currentState++;
    } else if(currentState == 2){
        PWM1_LoadDutyValue(0);
        PWM2_LoadDutyValue((float)(beatBrightness/9.0) * 120);
        PWM3_LoadDutyValue((float)(beatBrightness/9.0) * 120);
        PWM4_LoadDutyValue(0);
        currentState++;
    } else if(currentState == 3){
        PWM1_LoadDutyValue((float)(beatBrightness/9.0) * 120);
        PWM2_LoadDutyValue(0);
        PWM3_LoadDutyValue((float)(beatBrightness/9.0) * 120);
        PWM4_LoadDutyValue(0);
        currentState++;
    } else if(currentState == 4){
        PWM1_LoadDutyValue(0);
        PWM2_LoadDutyValue(0);
        PWM3_LoadDutyValue((float)(beatBrightness/9.0) * 120);
        PWM4_LoadDutyValue((float)(beatBrightness/9.0) * 120);
        currentState++;
    }else if(currentState == 5){
        PWM1_LoadDutyValue((float)(beatBrightness/9.0) * 120);
        PWM2_LoadDutyValue((float)(beatBrightness/9.0) * 120);
        PWM3_LoadDutyValue((float)(beatBrightness/9.0) * 120);
        PWM4_LoadDutyValue(0);
        currentState++;
    } else if(currentState == 6){
        PWM1_LoadDutyValue((float)(beatBrightness/9.0) * 120);
        PWM2_LoadDutyValue(0);
        PWM3_LoadDutyValue((float)(beatBrightness/9.0) * 120);
        PWM4_LoadDutyValue((float)(beatBrightness/9.0) * 120);
        currentState++;
    } else if(currentState == 8){
        PWM1_LoadDutyValue(0);
        PWM2_LoadDutyValue((float)(beatBrightness/9.0) * 120);
        PWM3_LoadDutyValue((float)(beatBrightness/9.0) * 120);
        PWM4_LoadDutyValue((float)(beatBrightness/9.0) * 120);
        currentState++;
    } else {
        PWM1_LoadDutyValue((float)(beatBrightness/9.0) * 120);
        PWM2_LoadDutyValue((float)(beatBrightness/9.0) * 120);
        PWM3_LoadDutyValue((float)(beatBrightness/9.0) * 120);
        PWM4_LoadDutyValue((float)(beatBrightness/9.0) * 120);
        currentState = 1;
    }
    
}



void static LED_task_ANIMATION(){
    
}
