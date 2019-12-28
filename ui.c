/*
 * File:   ui.c
 * Author: root
 *
 * Created on April 27, 2019, 1:41 PM
 */

#include "ui.h"

typedef void (*fn_t)(void);

typedef struct {
    menuItem_t* nextItem;
    menuItem_t* prevItem;
    char name[4];
    fn_t config;
} menuItem_t;

void UI_init();

void UI_init() {

    //Level 1 UI
    menuItem_t dmx;
    menuItem_t beat;
    menuItem_t animation;
    menuItem_t manual;
    
    dmx.nextItem = &beat;
    dmx.prevItem = &manual;
    dmx.config = NULL;
    dmx.name = "PC  ";

    dmx.nextItem = &beat;
    beat.nextItem = &animation;
    animation.nextItem = &manual;

    dmx.config = dmxAddressPrint;

    beat.config = beatConfig;



    beatStrobe.nextItem = &beatFade;

    // animation configs
    menuItem_t animationType;

    // manualConfigs;
    menuItem_t manualRed;

    dmx.nextItem = &beatStrobe;
    beat
}