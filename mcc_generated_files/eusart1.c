/**
  EUSART1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    eusart1.c

  @Summary
    This is the generated driver implementation file for the EUSART1 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This source file provides APIs for EUSART1.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.76
        Device            :  PIC16F18446
        Driver Version    :  2.1.0
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.00
        MPLAB 	          :  MPLAB X 5.10
 */

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
 */

/**
  Section: Included Files
 */
#include "eusart1.h"

/**
  Section: Macro Declarations
 */

#define EUSART1_TX_BUFFER_SIZE 8
#define EUSART1_RX_BUFFER_SIZE 8

/**
  Section: Global Variables
 */

int initial = 1;
int arrayIndex = 0;
int dmxArray[513];

/**
  Section: EUSART1 APIs
 */
void (*EUSART1_FramingErrorHandler)(void);
void (*EUSART1_OverrunErrorHandler)(void);
void (*EUSART1_ErrorHandler)(void);

void EUSART1_DefaultFramingErrorHandler(void);
void EUSART1_DefaultOverrunErrorHandler(void);
void EUSART1_DefaultErrorHandler(void);

void EUSART1_Initialize(void) {
    // disable interrupts before changing states
    PIE3bits.RC1IE = 0;
    EUSART1_SetRxInterruptHandler(EUSART1_Receive_ISR);
    // Set the EUSART1 module to the options selected in the user interface.

    // ABDOVF no_overflow; SCKP Non-Inverted; BRG16 16bit_generator; WUE disabled; ABDEN disabled; 
    BAUD1CON = 0x08;

    // SPEN enabled; RX9 8-bit; CREN enabled; ADDEN disabled; SREN disabled; 
    RC1STA = 0x90;

    // TX9 8-bit; TX9D 0; SENDB sync_break_complete; TXEN disabled; SYNC asynchronous; BRGH hi_speed; CSRC slave; 
    TX1STA = 0x04;

    // SP1BRGL 7; 
    SP1BRGL = 0x07;

    // SP1BRGH 0; 
    SP1BRGH = 0x00;


    EUSART1_SetFramingErrorHandler(EUSART1_DefaultFramingErrorHandler);
    EUSART1_SetOverrunErrorHandler(EUSART1_DefaultOverrunErrorHandler);
    EUSART1_SetErrorHandler(EUSART1_DefaultErrorHandler);

    // enable receive interrupt
    PIE3bits.RC1IE = 1;
}

bool EUSART1_is_tx_ready(void) {
    return (bool) (PIR3bits.TX1IF && TX1STAbits.TXEN);
}

bool EUSART1_is_tx_done(void) {
    return TX1STAbits.TRMT;
}

void EUSART1_Write(uint8_t txData) {
    while (0 == PIR3bits.TX1IF) {
    }

    TX1REG = txData; // Write the data byte to the USART.
}

void EUSART1_Receive_ISR(void) {
    if (RC1STAbits.OERR) {
        RC1STAbits.CREN = 0;
        RC1STAbits.CREN = 1;
    }

    // or set custom function using EUSART1_SetRxInterruptHandler()

    if (initial == 0 && !(RC1STAbits.FERR)) {
        dmxArray[arrayIndex] = RC1REG;
//        dmxArray[arrayIndex + 1] = RC1REG;
        arrayIndex += 1;
    } else {
        arrayIndex = 0;
        initial = 0;
        volatile uint8_t dummy = RC1REG;
    }

    PIR3bits.RC1IF = 0;

}

void EUSART1_DefaultFramingErrorHandler(void) {
}

void EUSART1_DefaultOverrunErrorHandler(void) {
    // EUSART1 error - restart

    RC1STAbits.CREN = 0;
    RC1STAbits.CREN = 1;

}

void EUSART1_DefaultErrorHandler(void) {
    EUSART1_RxDataHandler();
}

void EUSART1_SetFramingErrorHandler(void (* interruptHandler)(void)) {
    EUSART1_FramingErrorHandler = interruptHandler;
}

void EUSART1_SetOverrunErrorHandler(void (* interruptHandler)(void)) {
    EUSART1_OverrunErrorHandler = interruptHandler;
}

void EUSART1_SetErrorHandler(void (* interruptHandler)(void)) {
    EUSART1_ErrorHandler = interruptHandler;
}

void EUSART1_SetRxInterruptHandler(void (* interruptHandler)(void)) {
    EUSART1_RxDefaultInterruptHandler = interruptHandler;
}
/**
  End of File
 */
