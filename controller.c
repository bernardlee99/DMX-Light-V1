#include "clock.h"
#include "controller.h"
#include "buttons.h"
#include "tm1650.h"
#include "dmx.h"
#include "led.h"

button_t *up, *down, *menu, *enter;
int incInterval = MAX_INTERVAL_TIME;
time_t lastIncTime = 0;
time_t lastActiveTime = 0;
bool startup = true;

mode_t menuSelected = MODE_DMX;
mode_t currentSelection = MODE_DMX;
bool menuPressed = false;
bool enterPressed = false;

bool upState, downState, menuState, enterState;

void CONTROLLER_task() {

    bool isActive = true;
    upState = BUTTONS_isClicked(up);
    downState = BUTTONS_isClicked(down);
    menuState = BUTTONS_isClicked(menu);
    enterState = BUTTONS_isClicked(enter);

    if (startup == true && CLOCK_getTime() - startTime < 5000) {
        menuSelection();
        if (upState || downState || menuState) {
            menuPressed = true;
        }
        //if (enterState) {
        return;
        //}
    } else {
        if (menuPressed == false && startup == true)
            currentSelection = MODE_DMX;
        startup = false;
    }

    if ((menuState || menuPressed) && !enterPressed) {
        menuPressed = true;
        menuSelection();
    } else if (currentSelection == MODE_DMX) {
        isActive = CONTROL_DMX();
    } else if (currentSelection == MODE_ANIMATION) {
        isActive = true;
    } else if (currentSelection == MODE_BEAT_STROBE || currentSelection == MODE_BEAT_FADE) {
        isActive = CONTROL_BEAT();
    } else {
        isActive = false;
    }

    if (isActive) {
        lastActiveTime = CLOCK_getTime();
    }

    if (CLOCK_getTime() - lastActiveTime >= TURN_OFF_TIME) {
        TM1650_enable(false);
        lastActiveTime = CLOCK_getTime() - (TURN_OFF_TIME + 1);
    } else {
        TM1650_enable(true);
    }

    if (currentSelection == MODE_DMX && !menuPressed) {
        TM1650_fastPrintNum(address);
        enterPressed = false;
    } else if (currentSelection == MODE_ANIMATION && !menuPressed) {
        printf("SEL1\r");
        enterPressed = false;
    } else if (currentSelection == MODE_BEAT_STROBE && !menuPressed) {
        printf("B-%u  \r", beatBrightness);
        enterPressed = false;
    } else if (currentSelection == MODE_BEAT_FADE && !menuPressed) {
        printf("F-%u  \r", beatBrightness);
        enterPressed = false;
    }
    
}

void menuSelection(){
    
    if(menuSelected == MODE_BEAT_FADE)
        menuSelected = MODE_BEAT_STROBE;
    
    if(upState){
        if(menuSelected < 2){
            menuSelected++;
        } else {
            menuSelected = 2;
        }
    } else if(downState){
        if(menuSelected > 0){
            menuSelected--;
        } else {
            menuSelected = 0;
        }
    }

    if (enterState) {
        currentSelection = menuSelected;
        menuPressed = false;
        enterPressed = true;
        return;
    }
    
    switch(menuSelected){
        case MODE_ANIMATION:
            printf("ANI \r");
            break;
            
        case MODE_BEAT_STROBE:
            printf("BEAT\r");
            break;
            
        case MODE_BEAT_FADE:
            printf("BEAT\r");
            break;
            
        case MODE_DMX:
            printf("PC  \r");
            break;
    }
}

mode_t getMode(){
    return currentSelection;
}

bool static CONTROL_DMX(){
    
    if(BUTTONS_isHeld(up) && (CLOCK_getTime() - lastIncTime > incInterval)){
        address_inc();
        lastIncTime = CLOCK_getTime();
        if(incInterval > MIN_INTERVAL_TIME){
            incInterval-=INTERVAL_CONST;
        }
        return true;
    } else if(BUTTONS_isHeld(down) && (CLOCK_getTime() - lastIncTime > incInterval)){
        address_dec();
        lastIncTime = CLOCK_getTime();
        if(incInterval > MIN_INTERVAL_TIME){
            incInterval-=INTERVAL_CONST;
        }
        return true;
    } else if(BUTTONS_isHeld(enter)){
        address = 1;
        return true;
    } else if (upState) {
        address_inc();
        return true;
        lastActiveTime = CLOCK_getTime();        
    } else if (downState) {
        address_dec();
        return true;
    } else if(enterState){
        return true;
    } else if((!upState || !downState) && !(BUTTONS_isHeld(down) || BUTTONS_isHeld(up))){
        incInterval = MAX_INTERVAL_TIME;
    } else {
        return false;
    }
    
    return false;
    
}

bool static CONTROL_BEAT(){
    if (upState) {
        if(beatBrightness < 9){
            beatBrightness++;
        } else {
            beatBrightness = 9;
        }
        return true;     
    } else if (downState) {
        if(beatBrightness > 0){
            beatBrightness--;
        } else {
            beatBrightness = 0;
        }
        return true;     
    }
    
    if(enterState && currentSelection == MODE_BEAT_FADE){
        currentSelection = MODE_BEAT_STROBE;
    } else if(enterState && currentSelection == MODE_BEAT_STROBE){
        currentSelection = MODE_BEAT_FADE;
    }
}