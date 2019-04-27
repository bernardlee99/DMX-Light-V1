#include "clock.h"
#include "controller.h"
#include "buttons.h"
#include "tm1650.h"
#include "dmx.h"

button_t *up, *down, *menu, *enter;
int incInterval = MAX_INTERVAL_TIME;
time_t lastIncTime = 0;
time_t lastActiveTime = 0;
bool startup = true;

uint8_t menuSelected = 3;
uint8_t currentSelection = 3;
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
            currentSelection = 3;
        startup = false;
    }

    if ((menuState || menuPressed) && !enterPressed) {
        menuPressed = true;
        menuSelection();
    } else if (currentSelection == 3) {
        isActive = CONTROL_DMX();
    } else if (currentSelection == 1) {
        isActive = true;
    } else if (currentSelection == 2) {
        isActive = true;
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

    if (currentSelection == 3 && !menuPressed) {
        TM1650_fastPrintNum(address);
        enterPressed = false;
    } else if (currentSelection == 1 && !menuPressed) {
        printf("SEL1\r");
        enterPressed = false;
    } else if (currentSelection == 2 && !menuPressed) {
        printf("B-1 \r");
        enterPressed = false;
    }
    
}

void menuSelection(){
    
    if(upState){
        if(menuSelected < 3){
            menuSelected++;
        } else {
            menuSelected = 2;
        }
    } else if(downState){
        if(menuSelected > 1){
            menuSelected--;
        } else {
            menuSelected = 1;
        }
    }

    if (enterState) {
        currentSelection = menuSelected;
        menuPressed = false;
        enterPressed = true;
        return;
    }
    
    switch(menuSelected){
        case 1:
            printf("ANI \r");
            break;
            
        case 2:
            printf("BEAT\r");
            break;
            
        case 3:
            printf("PC  \r");
            break;
    }
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