/*
 * File:   dmx.c
 * Author: student
 *
 * Created on April 26, 2019, 6:22 PM
 */


#include "dmx.h"
#include "tm1650.h"
#include "mcc_generated_files/eusart1.h"

void DMX_interrupt();

int initial = 1;
int arrayIndex = 0;
int dmxArray[513];

uint16_t address = 1;

void DMX_init(){
    EUSART1_SetRxInterruptHandler(DMX_interrupt);
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
    //TM1650_fastPrintNum(address);
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
    //TM1650_fastPrintNum(address);
}

void DMX_interrupt(){
    
    if (RC1STAbits.OERR) {
        RC1STAbits.CREN = 0;
        RC1STAbits.CREN = 1;
    }

    if (initial == 0 && !(RC1STAbits.FERR)) {
        dmxArray[arrayIndex] = RC1REG;
        arrayIndex += 1;
    } else {
        arrayIndex = 0;
        initial = 0;
        volatile uint8_t dummy = RC1REG;
    }

    PIR3bits.RC1IF = 0;
}

uint8_t DMX_getAddress(){
    return address;
}