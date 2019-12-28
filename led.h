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

typedef struct {
	uint16_t hue;
	float saturation;
	float light;
}hsl_t;

void LED_init();
void LED_task();
void LED_task_DMX();
void LED_task_BEAT_STROBE();
void LED_task_BEAT_FADE();
void LED_task_ANIMATION(uint8_t mode, uint8_t brightness, uint8_t speed);
color_t static beatColorCreator(bool inRed, bool inGreen, bool inBlue, bool inWhite);
color_t static colorCreator(uint8_t inRed, uint8_t inGreen, uint8_t inBlue, uint8_t inWhite);
float static beatBrightnessCalculation();
void LED_task_BEAT_CONTINUOUS();
void LED_task_BEAT_MIXED();
void LED_task_MANUAL();


void colorDec();
void colorInc();
uint8_t getManualColor(colormode_t input);

extern uint8_t beatBrightness;
extern uint8_t animationBrightness;

extern color_t manualColor;

float HueToRGB(float v1, float v2, float vH);
color_t HSLToRGB(hsl_t hsl);
color_t animationColor;
hsl_t animationHue;

#endif