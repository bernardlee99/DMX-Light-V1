#include <stdint.h>
#include<stdbool.h>
#include "clock.h"
#include "controller.h"
#include "buttons.h"
#include "tm1650.h"

uint16_t address = 1;
button_t *up, *down, *menu, *enter;
int incInterval = MAX_INTERVAL_TIME;
time_t lastIncTime = 0;
time_t lastActiveTime = 0;

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
    
    if(CLOCK_getTime() - lastActiveTime > TURN_OFF_TIME){
        TM1650_enable(false);
    } else {
        TM1650_enable(true);
    }
    
    if (BUTTONS_isClicked(up)) {
        address_inc();
        lastActiveTime = CLOCK_getTime();        
    } else if (BUTTONS_isClicked(down)) {
        address_dec();
        lastActiveTime = CLOCK_getTime();
    }

    if(BUTTONS_isHeld(up) && (CLOCK_getTime() - lastIncTime > incInterval)){
        address_inc();
        lastIncTime = CLOCK_getTime();
        if(incInterval > MIN_INTERVAL_TIME){
            incInterval-=INTERVAL_CONST;
        }
        lastActiveTime = CLOCK_getTime();
    } else if(BUTTONS_isHeld(down) && (CLOCK_getTime() - lastIncTime > incInterval)){
        address_dec();
        lastIncTime = CLOCK_getTime();
        if(incInterval > MIN_INTERVAL_TIME){
            incInterval-=INTERVAL_CONST;
        }
        lastActiveTime = CLOCK_getTime();
    } else if(BUTTONS_isHeld(enter)){
        address = 1;
    } else if(!BUTTONS_isHeld(up) && !BUTTONS_isHeld(down)){
        incInterval = MAX_INTERVAL_TIME;
    } 
    
}
