/*
 * File:   ui.c
 * Author: root
 *
 * Created on April 27, 2019, 1:41 PM
 */

#include "ui.h"

uint8_t menuSelected = 3;
uint8_t currentSelection = 3;

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