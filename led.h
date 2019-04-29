#ifndef STD_H
#define STD_H
#include<stdbool.h>
#include<stdint.h>
#include <xc.h>
#endif

#ifndef LED_H
#define LED_H

typedef struct {
    uint8_t white;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
}color_t;

void LED_init();
void LED_task();
void static LED_task_DMX();
void static LED_task_BEAT_STROBE();
void static LED_task_BEAT_FADE();
void static LED_task_ANIMATION();
color_t static beatColorCreator(bool inRed, bool inGreen, bool inBlue, bool inWhite);
color_t static colorCreator(uint8_t inRed, uint8_t inGreen, uint8_t inBlue, uint8_t inWhite);
float static beatBrightnessCalculation();
void static LED_task_BEAT_CONTINUOUS();
void static LED_task_BEAT_MIXED();

void colorDec(colormode_t input);
void colorInc(colormode_t input);
uint8_t getManualColor(colormode_t input);

extern uint8_t beatBrightness;

#endif