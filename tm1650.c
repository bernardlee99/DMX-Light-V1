/*
 * File:   tm1650.c
 * Author: berna
 *
 * Created on March 16, 2019, 5:49 PM
 */


#include <xc.h>
#include <string.h>
#include<stdio.h>
#include"tm1650.h"
#include "mcc_generated_files/i2c1.h"

bool tm1650_status = false;

#define _XTAL_FREQ 4000000
#define SPEED_SCROLL 2000

const uint8_t charTable[] = 
{
    0x00, 0x82, 0x21, 0x00, 0x00, 0x00, 0x00, 0x02, 0x39, 0x0F, 0x00, 0x00, 0x00, 0x40, 0x80, 0x00,
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7f, 0x6f, 0x00, 0x00, 0x00, 0x48, 0x00, 0x53,
    0x00, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x6F, 0x76, 0x06, 0x1E, 0x00, 0x38, 0x00, 0x54, 0x3F,
    0x73, 0x67, 0x50, 0x6D, 0x78, 0x3E, 0x00, 0x00, 0x00, 0x6E, 0x00, 0x39, 0x00, 0x0F, 0x00, 0x08, 
    0x63, 0x5F, 0x7C, 0x58, 0x5E, 0x7B, 0x71, 0x6F, 0x74, 0x02, 0x1E, 0x00, 0x06, 0x00, 0x54, 0x5C,
    0x73, 0x67, 0x50, 0x6D, 0x78, 0x1C, 0x00, 0x00, 0x00, 0x6E, 0x00, 0x39, 0x30, 0x0F, 0x00, 0x00 
};

uint8_t position = 0;

static void writeData(uint8_t address, uint8_t data){
    
    I2C1_MESSAGE_STATUS status = I2C1_MESSAGE_PENDING;
    I2C1_MasterWrite(&data, 1, address, &status);
    
    while( status == I2C1_MESSAGE_PENDING );
    
    return;
}

void TM1650_enable(bool enable){
    if(enable){
        writeData(0x24, 0x01);          //Display ON command
        tm1650_status = true;
    } else {
        writeData(0x24, 0x00);          //Display OFF command
        tm1650_status = false;
    }
}

void TM1650_init(){
    
    TM1650_enable(true);
    printf("    ");
    printf("\r");
    
}

void TM1650_brightness(uint8_t brightness){
    
    writeData(0x24, (brightness << 4) | 0x00);          //Brightness 4
}

void TM1650_scrollPrint(char *array){
    
    printf("   %c\r", array[0]);
    __delay_ms(SPEED_SCROLL);
    printf("  %c%c\r", array[0], array[1]);
    __delay_ms(SPEED_SCROLL);
    printf(" %c%c%c\r", array[0], array[1], array[2]);
    __delay_ms(SPEED_SCROLL);
    
    for(int i = 0; i < strlen(array) - 3; i++){
        printf("%c%c%c%c\r", array[i], array[i+1], array[i+2], array[i+3]);
        __delay_ms(SPEED_SCROLL);
    }
    
    printf("%c%c%c \r", array[strlen(array) - 3], array[strlen(array) - 2], array[strlen(array) - 1]);
    __delay_ms(SPEED_SCROLL);
    printf("%c%c  \r", array[strlen(array) - 2], array[strlen(array) - 1]);
    __delay_ms(SPEED_SCROLL);
    printf("%c   \r", array[strlen(array) - 1]);
    __delay_ms(SPEED_SCROLL);
    printf("    \r", array[strlen(array) - 1]);
    __delay_ms(SPEED_SCROLL);  
    
    
}

void TM1650_setDigit(uint8_t digit, uint8_t data){
    
    writeData(digit + 0x34, charTable[data - 32]);
    
}

void TM1650_fastPrintNum(uint16_t num){
    
    if(num > 9999){
        writeData(0x34, 0x79);
        writeData(0x35, 0x79);
        writeData(0x36, 0x79);
        writeData(0x37, 0x79);
        return;
    }
        
    writeData(0x37, charTable[(num % 10) + 16]);
    writeData(0x36, charTable[( (num/10) % 10) + 16]);
    writeData(0x35, charTable[( (num/100) % 10) + 16]);
    writeData(0x34, charTable[( (num/1000) % 10) + 16]);
    
    
    
}

void putch(char n){

    if(position > 3 || n == '\r' || n == '\0')
        position = 0;
    
    if(n != '\r'){  
        TM1650_setDigit(position, n);
        position++;
    }
    
}

bool TM1650_isEnabled(){
    return tm1650_status;
}



