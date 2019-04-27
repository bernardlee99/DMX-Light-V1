/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.76
        Device            :  PIC16F18446
        Driver Version    :  2.00
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

#include <pic16f18446.h>
#include<stdint.h>
#include "tm1650.h"
#include "clock.h"
#include "buttons.h"
#include "controller.h"
#include "led.h"
#include "beat.h"
#include "mcc_generated_files/mcc.h"
#include "beat.h"
#include "dmx.h"
int dmxArray[513];

button_t *up, *down, *menu, *enter;

/*
                         Main application
 */

void main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    __delay_ms(500);
    CLOCK_init();
    BUTTONS_init();
    DMX_init();

    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
    
    TM1650_init();
    CONTROLLER_init();
    BEAT_init();
    
    
    while (1)
    {   
        BUTTONS_task();
        CONTROLLER_task();
        BEAT_task();
        LED_task();

            /*PWM1_LoadDutyValue( ( ((int)dmxArray[getAddress()])/255.0 ) * 120);

            PWM2_LoadDutyValue( ( ((int)dmxArray[1+getAddress()])/255.0 ) * 120);

            PWM3_LoadDutyValue( ( ((int)dmxArray[2+getAddress()])/255.0 ) * 120);

            PWM4_LoadDutyValue( ( ((int)dmxArray[3+getAddress()])/255.0 ) * 120);*/
            
    }
}
/**
 End of File
*/