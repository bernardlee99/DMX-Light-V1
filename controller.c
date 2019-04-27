#include "clock.h"
#include "controller.h"
#include "buttons.h"
#include "tm1650.h"


uint16_t address = 1;
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

void CONTROLLER_init() {
    TM1650_fastPrintNum(address);
}

/**
 * Increments the address and updates the display
 */
void address_inc()
{
    if(address == 512)
        address = 1;
    else
        address++;
    
    // update the display
    TM1650_fastPrintNum(address);
}

/**
 * Decrements the address and updates the display
 */
void address_dec() 
{
    if(address == 1)
        address = 512;
    else
        address--;  
    
    // update the display
    TM1650_fastPrintNum(address);
}

void CONTROLLER_task() {
    
    bool isActive = true;
    upState = BUTTONS_isClicked(up);
    downState = BUTTONS_isClicked(down);
    menuState = BUTTONS_isClicked(menu);
    enterState = BUTTONS_isClicked(enter);
    

    
    if(startup == true && CLOCK_getTime() - startTime < 5000){
        menuSelection();
        if (upState || downState || menuState) {
            menuPressed = true;
        }
        //if (enterState) {
            return;
        //}
    } else {
        if(menuPressed == false && startup == true)
            currentSelection = 3;
        startup = false;
    }
    
     volatile uint8_t status = currentSelection;
    
    if((menuState || menuPressed) && !enterPressed){
        menuPressed = true;
        menuSelection();        
    } else if (upState && currentSelection == 3) {
        address_inc();
        lastActiveTime = CLOCK_getTime();        
    } else if (downState && currentSelection == 3) {
        address_dec();
    } else if(upState && (CLOCK_getTime() - lastIncTime > incInterval) && currentSelection == 3){
        address_inc();
        lastIncTime = CLOCK_getTime();
        if(incInterval > MIN_INTERVAL_TIME){
            incInterval-=INTERVAL_CONST;
        }
    } else if(downState && (CLOCK_getTime() - lastIncTime > incInterval) && currentSelection == 3){
        address_dec();
        lastIncTime = CLOCK_getTime();
        if(incInterval > MIN_INTERVAL_TIME){
            incInterval-=INTERVAL_CONST;
        }
    } else if(enterState && currentSelection == 3){
        address = 1;
    } else {
        isActive = false;
    }
    
    if (currentSelection == 1 || currentSelection == 2){
        isActive = true;
    }
    
    if(!upState && !downState){
        incInterval = MAX_INTERVAL_TIME;
    } 
    
    if(isActive) {
        lastActiveTime = CLOCK_getTime();
    }
    
    if (CLOCK_getTime() - lastActiveTime >= TURN_OFF_TIME) {
        TM1650_enable(false);
        lastActiveTime = CLOCK_getTime() - (TURN_OFF_TIME + 1);
    } else{
        TM1650_enable(true);
    }
    
    if(currentSelection == 3 && !menuPressed) {
        TM1650_fastPrintNum(address);
        enterPressed = false;
    } else if(currentSelection == 1 && !menuPressed) {
        printf("SEL1\r");
        enterPressed = false;
    } else if(currentSelection == 2 && !menuPressed) {
        printf("B-1 \r");
        enterPressed = false;
    }
    
}

uint8_t getAddress(){
    return address;
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