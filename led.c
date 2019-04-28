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
        PWM1_LoadDutyValue( ( ((int)input.red)/255.0 ) * 127 );
        PWM2_LoadDutyValue( ( ((int)input.green)/255.0 ) * 127 );
        PWM3_LoadDutyValue( ( ((int)input.blue)/255.0 ) * 127 );
        PWM4_LoadDutyValue( ( ((int)input.white)/255.0 ) * 127 );
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
            
        case MODE_BEAT_STROBE:
            LED_task_BEAT_STROBE();
            break;
            
        case MODE_BEAT_FADE:
            LED_task_BEAT_FADE();
            break;
            
        case MODE_ANIMATION:
            LED_task_ANIMATION();
            break;
    }
    
}

void static LED_task_BEAT_STROBE(){
    
    if(CLOCK_getTime() - lastLedActiveTime < 50){
        return;
    }
        
    lastLedActiveTime = CLOCK_getTime();
    
     if(!BEAT_detected()){
         LED_setColor(beatColorCreator(0,0,0,0));
    } else if (currentState == 1){
        LED_setColor(beatColorCreator(0,0,0,1));
        currentState++;
    } else if(currentState == 2){
         LED_setColor(beatColorCreator(0,1,0,1));
        currentState++;
    } else if(currentState == 3){
         LED_setColor(beatColorCreator(0,1,1,0));
        currentState++;
    } else if(currentState == 4){
         LED_setColor(beatColorCreator(1,0,1,1));
        currentState++;
    }else if(currentState == 5){
        LED_setColor(beatColorCreator(0,1,1,0));
        currentState++;
    } else if(currentState == 6){
        LED_setColor(beatColorCreator(1,1,1,0));
        currentState++;
    } else if(currentState == 8){
        LED_setColor(beatColorCreator(1,1,0,1));
        currentState++;
    } else if(currentState == 9){
         LED_setColor(beatColorCreator(0,1,0,1));
        currentState++;
    } else if(currentState == 10){
         LED_setColor(beatColorCreator(0,0,1,1));
        currentState++;
    } else if(currentState == 11){
         LED_setColor(beatColorCreator(1,0,1,0));
        currentState++;
    }else if(currentState == 12){
        LED_setColor(beatColorCreator(1,0,0,1));
        currentState++;
    } else if(currentState == 13){
        LED_setColor(beatColorCreator(1,1,1,1));
        currentState++;
    } else if(currentState == 14){
        LED_setColor(beatColorCreator(0,1,0,1));
        currentState++;
    } else {
        LED_setColor(beatColorCreator(0,1,1,1));
        currentState = 1;
    }
    
}

float static beatBrightnessCalculation(){
    uint8_t temp = (float)(beatBrightness/9.0) * 255;
    return temp;
}

color_t static colorCreator(uint8_t inRed, uint8_t inGreen, uint8_t inBlue, uint8_t inWhite){
    color_t tempColor;
    tempColor.red = inRed;
    tempColor.blue = inBlue;
    tempColor.green = inGreen;
    tempColor.white = inWhite;
    
    return tempColor;
}

color_t static beatColorCreator(bool inRed, bool inGreen, bool inBlue, bool inWhite){
    color_t tempColor;
    
    if(inRed)
        tempColor.red = beatBrightnessCalculation();
    else
        tempColor.red = 0;
    
    if(inBlue)
        tempColor.blue = beatBrightnessCalculation();
    else
        tempColor.blue = 0;
    
    if(inGreen)
        tempColor.green = beatBrightnessCalculation();
    else
        tempColor.green = 0;
    
    if(inWhite)
        tempColor.white = beatBrightnessCalculation();
    else
        tempColor.white = 0;
    
    return tempColor;
}

void static LED_task_BEAT_FADE(){
    
    
}

void static LED_task_ANIMATION(){
    
}
