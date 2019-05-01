/*
 * File:   led.c
 * Author: student
 *
 * Created on April 26, 2019, 6:34 PM
 */


#include "beat.h"
#include "clock.h"
#include "controller.h"
#include "led.h"
#include "mcc_generated_files/mcc.h"
#include "dmx.h"
#include "tm1650.h"

time_t lastLedActiveTime = 0;
int currentState = 0;
uint8_t beatBrightness = 5;
uint8_t animationBrightness = 5;
 int dmxArray[513];
 bool startup;
 
 color_t manualColor;
 
void LED_setColor(color_t input){

        PWM1_LoadDutyValue( ( ((int)input.red)/255.0 ) * 127 );
        PWM2_LoadDutyValue( ( ((int)input.green)/255.0 ) * 127 );
        PWM3_LoadDutyValue( ( ((int)input.blue)/255.0 ) * 127 );
        PWM4_LoadDutyValue( ( ((int)input.white)/255.0 ) * 127 );

}

void LED_task_DMX(){
    
    color_t dmx;

    dmx.red = dmxArray[DMX_getAddress()];
    dmx.green = dmxArray[DMX_getAddress() + 1];
    dmx.blue = dmxArray[DMX_getAddress() + 2];
    dmx.white = dmxArray[DMX_getAddress() + 3];
    
    LED_setColor(dmx);

}

void LED_init(){
    manualColor.red = 0;
    manualColor.green = 0;
    manualColor.blue = 0;
    manualColor.white = 0;
    TRISCbits.TRISC5 = 0;
}

void LED_task_MANUAL(){

    LED_setColor(manualColor);
    
}

void LED_task_BEAT_STROBE(){
    
    if(CLOCK_getTime() - lastLedActiveTime < 50){
        return;
    }
    
    lastLedActiveTime = CLOCK_getTime();
    
    LATC5 = 0;
    bool beatState = BEAT_detected();
    
    if (beatState) {
        LATC5 = 1;
    }
    
     if(!beatState){
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

void resetColor(){
    beatColorCreator(0,0,0,0);
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

void LED_task_BEAT_FADE(){
        
}

void LED_task_ANIMATION(){
    
}

void LED_task_BEAT_CONTINUOUS(){
    
}

void LED_task_BEAT_MIXED(){
    
}

void colorInc(){
    
    switch(getCurrentMenu()){
        
        case MANUAL_RED:
            if(!(manualColor.red > 254))
            manualColor.red++;
            break;
            
        case MANUAL_GREEN:
            if(!(manualColor.green > 254))
            manualColor.green++;
            break;
            
        case MANUAL_BLUE:
            if(!(manualColor.blue > 254))
            manualColor.blue++;
            break;
            
        case MANUAL_WHITE:
            if(!(manualColor.white > 254))
            manualColor.white++;
            break;
              
    }
    
}

void colorDec(){
    
    switch(getCurrentMenu()){
        
        case MANUAL_RED:
            if(!(manualColor.red < 1))
            manualColor.red--;
            break;
            
        case MANUAL_GREEN:
            if(!(manualColor.green < 1))
            manualColor.green--;
            break;
            
        case MANUAL_BLUE:
            if(!(manualColor.blue < 1))
            manualColor.blue--;
            break;
            
        case MANUAL_WHITE:
            if(!(manualColor.white < 1))
            manualColor.white--;
            break;
              
    }
    
}

uint8_t getManualColor(colormode_t input){
    
       switch(input){
        
        case CMODE_RED:
            return manualColor.red;
            break;
            
        case CMODE_GREEN:
            return manualColor.green;
            break;
            
        case CMODE_BLUE:
            return manualColor.blue;
            break;
            
        case CMODE_WHITE:
            return manualColor.white;
            break;
              
    }
    
}