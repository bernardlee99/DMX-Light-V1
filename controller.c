#include "clock.h"
#include<stdio.h>
#include "controller.h"
#include "buttons.h"
#include "tm1650.h"
#include "dmx.h"
#include "led.h"

typedef void (*fn_t)(void);

struct menuItem;

 typedef struct menuItem {
    struct menuItem* nextItem;
    struct menuItem* prevItem;
    struct menuItem* config;
    struct menuItem* parent;
    menu_t name;
    fn_t task;
} menuItem_t;


//Main-Menu
menuItem_t dmx;
menuItem_t beat;
menuItem_t animation;
menuItem_t manual;

//DMX
menuItem_t dmx_task;

//Beat-Config
menuItem_t beatStrobe;
menuItem_t beatFade;
menuItem_t beatCont;
menuItem_t beatMixed;

menuItem_t beatStrobeTask;
menuItem_t beatFadeTask;
menuItem_t beatContTask;
menuItem_t beatMixedTask;

//Animation-Config
menuItem_t animationType;
menuItem_t animationBrightnessCtrl;

//Manual Config
menuItem_t manualRed;
menuItem_t manualGreen;
menuItem_t manualBlue;
menuItem_t manualWhite;

menuItem_t* currentMenu;

fn_t previousTask = NULL;

button_t *up, *down, *menu, *enter;
bool upState, downState, menuState, enterState;

//DMX increment 
int incInterval = MAX_INTERVAL_TIME;
time_t lastIncTime = 0;
time_t lastActiveTime = 0;
bool startup = true;

mode_t menuSelected = MODE_DMX;
mode_t currentSelection = MODE_DMX;
bool menuPressed = false;
bool enterPressed = false;

bool beatHold = false;

colormode_t colorModeSelected = CMODE_RED;

uint8_t animationModeSelected = 1;
bool animationBrightnessControl = false;
uint8_t animationBrightness;
    
void CONTROLLER_init() {

    //Level 1 UI
    dmx.nextItem = &beat;
    dmx.prevItem = &manual;
    dmx.config = &dmx_task;
    //dmx.task = previousTask;
    dmx.name = DMX;
    dmx.parent = &dmx;
    
    beat.nextItem = &animation;
    beat.prevItem = &dmx;
    beat.config = &beatStrobe;
    //beat.task = previousTask;
    beat.name = BEAT;
    beat.parent = &beat;
    
    animation.nextItem = &manual;
    animation.prevItem = &beat;
    animation.config = &animationBrightnessCtrl;
    //animation.task = previousTask;
    animation.name = ANIMATION;
    animation.parent = &animation;
    
    manual.nextItem = &dmx;
    manual.prevItem = &animation;
    manual.config = &manualRed;
    //manual.task = previousTask;
    manual.name = MANUAL;
    manual.parent = &manual;
    
    
    
    //Level 2 UI
    
    //DMX
    dmx_task.nextItem = NULL;
    dmx_task.prevItem = NULL;
    dmx_task.config = NULL;
    dmx_task.task = CONTROL_DMX;
    dmx_task.name = DMX_TASK;
    dmx_task.parent = &dmx;
    
    //Beat
    beatStrobe.nextItem = &beatFade;
    beatStrobe.prevItem = &beatMixed;
    beatStrobe.config = &beatStrobeTask;
    //beatStrobe.task = previousTask;
    beatStrobe.name = B_STROBE;
    beatStrobe.parent = &beat;
    
    beatFade.nextItem = &beatCont;
    beatFade.prevItem = &beatStrobe;
    beatFade.config = &beatFadeTask;
    //beatFade.task = previousTask;
    beatFade.name = B_FADE;
    beatFade.parent = &beat;
    
    beatCont.nextItem = &beatMixed;
    beatCont.prevItem = &beatFade;
    beatCont.config = &beatContTask;
    //beatCont.task = previousTask;
    beatCont.name = B_CONT;
    beatCont.parent = &beat;
    
    beatMixed.nextItem = &beatStrobe;
    beatMixed.prevItem = &beatCont;
    beatMixed.config = &beatMixedTask;
    //beatMixed.task = previousTask;
    beatMixed.name = B_MIXED;
    beatMixed.parent = &beat;
    
    
    //Animation
    animationType.nextItem = &animationBrightnessCtrl;
    animationType.prevItem = &animationBrightnessCtrl;
    animationType.config = NULL;
    animationType.task = LED_task_ANIMATION;
    animationType.name = A_SELECTION;
    animationType.parent = &animation;
    
    animationBrightnessCtrl.nextItem = &animationType;
    animationBrightnessCtrl.prevItem = &animationType;
    animationBrightnessCtrl.config = NULL;
    animationBrightnessCtrl.task = LED_task_ANIMATION;
    animationBrightnessCtrl.name = A_BRIGHTNESS;
    animationBrightnessCtrl.parent = &animation;
    
    
    //Manual
    manualRed.nextItem = &manualGreen;
    manualRed.prevItem = &manualWhite;
    manualRed.config = NULL;
    manualRed.task = LED_task_MANUAL;
    manualRed.name = MANUAL_RED;
    
    manualGreen.nextItem = &manualBlue;
    manualGreen.prevItem = &manualRed;
    manualGreen.config = NULL;
    manualGreen.task = LED_task_MANUAL;
    manualGreen.name = MANUAL_GREEN;
    
    manualBlue.nextItem = &manualWhite;
    manualBlue.prevItem = &manualGreen;
    manualBlue.config = NULL;
    manualBlue.task = LED_task_MANUAL;
    manualBlue.name = MANUAL_BLUE;
    
    manualWhite.nextItem = &manualRed;
    manualWhite.prevItem = &manualBlue;
    manualWhite.config = NULL;
    manualWhite.task = LED_task_MANUAL;
    manualWhite.name = MANUAL_WHITE;
    
    //Level 3 UI
    beatStrobeTask.nextItem = NULL;
    beatStrobeTask.prevItem = NULL;
    beatStrobeTask.config = NULL;
    beatStrobeTask.task = LED_task_BEAT_STROBE;
    beatStrobeTask.name = B_STROBE_TASK;
    
    beatFadeTask.nextItem = NULL;
    beatFadeTask.prevItem = NULL;
    beatFadeTask.config = NULL;
    beatFadeTask.task = LED_task_BEAT_STROBE;
    beatFadeTask.name = B_FADE_TASK;
    
    beatContTask.nextItem = NULL;
    beatContTask.prevItem = NULL;
    beatContTask.config = NULL;
    beatContTask.task = LED_task_BEAT_STROBE;
    beatContTask.name = B_CONT_TASK;
    
    beatMixedTask.nextItem = NULL;
    beatMixedTask.prevItem = NULL;
    beatMixedTask.config = NULL;
    beatMixedTask.task = LED_task_BEAT_STROBE;
    beatMixedTask.name = B_MIXED_TASK;

}

void CONTROLLER_task(){
    
    if(startup == true){
        currentMenu = &dmx;
        startup = false;
    }
    
    upState = BUTTONS_isClicked(up);
    downState = BUTTONS_isClicked(down);
    menuState = BUTTONS_isClicked(menu);
    enterState = BUTTONS_isClicked(enter);
    
    if(menuState){
        currentMenu = currentMenu->parent;
    } else if(upState && currentMenu->nextItem != NULL){
        currentMenu = currentMenu->nextItem;
    } else if(downState && currentMenu->prevItem != NULL){
        currentMenu = currentMenu->prevItem;
    } else if(enterState &&  currentMenu->config != NULL){
        currentMenu = currentMenu->config;
    } 
    
    switch(currentMenu->name){
        case DMX:
            printf("PC  \r");
            break;
            
        case BEAT:
            printf("BEAT\r");
            break;
            
        case ANIMATION:
            printf("ANI \r");
            break;
            
        case MANUAL:
            printf("LED \r");
            break;

        case B_STROBE:
            printf("B   \r");
            break;
            
        case B_FADE:
            printf("FADE\r");
            break;
            
        case B_CONT:
            printf("C   \r");
            break;
            
        case B_MIXED:
            printf("ALL \r");
            break;
            
        default:
            currentMenu->task();
            break;
    }
    
}

/*void CONTROLLER_task() {

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
        if (enterState) {
            return;
        }
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
        isActive = CONTROL_ANIMATION();
    } else if (currentSelection == MODE_BEAT_STROBE || currentSelection == MODE_BEAT_FADE || currentSelection == MODE_BEAT_CONTINUOUS || currentSelection == MODE_BEAT_MIXED) {
        isActive = CONTROL_BEAT();
    } else if(currentSelection == MODE_MANUAL && !enterPressed){
        isActive = CONTROL_MANUAL(colorModeSelected);
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
    } else if(beatHold == true && !menuPressed){
        printf("HOLD\r");
        lastActiveTime = CLOCK_getTime();
        enterPressed = false;
    } else if (currentSelection == MODE_ANIMATION && !menuPressed && !beatHold) {
        if (animationBrightnessControl) {
            printf("BR");
            TM1650_fastPrintNum_2digit(animationBrightness);
            printf("\r");
        } else {
            printf("AN");
            TM1650_fastPrintNum_2digit(animationModeSelected);
            printf("\r");
        }
        enterPressed = false;
    } else if (currentSelection == MODE_BEAT_STROBE && !menuPressed&& !beatHold) {
        printf("B-");
        TM1650_fastPrintNum_2digit((uint8_t)beatBrightness);
        printf("\r");
        enterPressed = false;
    } else if (currentSelection == MODE_BEAT_FADE && !menuPressed && !beatHold) {
        printf("F-");
        TM1650_fastPrintNum_2digit((uint8_t)beatBrightness);
        printf("\r");
        enterPressed = false;
    } else if (currentSelection == MODE_BEAT_CONTINUOUS && !menuPressed && !beatHold) {
        printf("C-");
        TM1650_fastPrintNum_2digit((uint8_t)beatBrightness);
        printf("\r");
        enterPressed = false;
    }  else if (currentSelection == MODE_BEAT_MIXED && !menuPressed && !beatHold) {
        printf("A-");
        TM1650_fastPrintNum_2digit((uint8_t)beatBrightness);
        printf("\r");
        enterPressed = false;
    } else if (currentSelection == MODE_MANUAL && !menuPressed){
        switch(colorModeSelected){
            case CMODE_RED:
                printf("r");
                TM1650_fastPrintNum_3digit(getManualColor(CMODE_RED));
                printf("\r");
                break;
                
            case CMODE_GREEN:
                printf("G");
                TM1650_fastPrintNum_3digit(getManualColor(CMODE_GREEN));
                printf("\r");
                break;
                
            case CMODE_BLUE:
                printf("B");
                TM1650_fastPrintNum_3digit(getManualColor(CMODE_BLUE));
                printf("\r");
                break;
                
            case CMODE_WHITE:
                printf("H");
                TM1650_fastPrintNum_3digit(getManualColor(CMODE_WHITE));
                printf("\r");
                break;
        }
        enterPressed = false;
    }
    
}*/

/*void menuSelection(){
    
    if(menuSelected == MODE_BEAT_FADE || menuSelected == MODE_BEAT_CONTINUOUS || menuSelected == MODE_BEAT_MIXED)
        menuSelected = MODE_BEAT_STROBE;
    
    if(upState){
        if(menuSelected < 3){
            menuSelected++;
        } else {
            menuSelected = 3;
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
            
        case MODE_MANUAL:
            printf("LED \r");
            break;
    }
}*/

mode_t getMode(){
    return currentSelection;
}

void static CONTROL_DMX(){

    printf("d\r");
    TM1650_fastPrintNum_3digit(DMX_getAddress());
    
    if(BUTTONS_isHeld(up) && (CLOCK_getTime() - lastIncTime > incInterval)){
        address_inc();
        lastIncTime = CLOCK_getTime();
        if(incInterval > MIN_INTERVAL_TIME){
            incInterval-=INTERVAL_CONST;
        }
    } else if(BUTTONS_isHeld(down) && (CLOCK_getTime() - lastIncTime > incInterval)){
        address_dec();
        lastIncTime = CLOCK_getTime();
        if(incInterval > MIN_INTERVAL_TIME){
            incInterval-=INTERVAL_CONST;
        }
    } else if(BUTTONS_isHeld(enter)){
        address = 1;
    } else if (upState) {
        address_inc();
        lastActiveTime = CLOCK_getTime();        
    } else if (downState) {
        address_dec();
    } else if((!upState || !downState) && !(BUTTONS_isHeld(down) || BUTTONS_isHeld(up))){
        incInterval = MAX_INTERVAL_TIME;
    } else {
    }
    
    
}

void static CONTROL_BEAT(){
    
    /*if(currentMenu.name == B_STROBE_TASK && !beatHold){
        LED_task_BEAT_STROBE();
        printf("B-");
        TM1650_fastPrintNum_2digit((uint8_t)beatBrightness);
        printf("\r");
    } else if (currentMenu.name == B_FADE_TASK && !beatHold) {
        LED_task_BEAT_FADE();
        printf("F-");
        TM1650_fastPrintNum_2digit((uint8_t)beatBrightness);
        printf("\r");
    } else if (currentMenu->name == B_CONT_TASK && !beatHold) {
        LED_task_BEAT_CONTINUOUS();
        printf("C-");
        TM1650_fastPrintNum_2digit((uint8_t)beatBrightness);
        printf("\r");
    }  else if (currentMenu->name == B_MIXED_TASK && !beatHold) {
        LED_task_BEAT_MIXED();
        printf("A-");
        TM1650_fastPrintNum_2digit((uint8_t)beatBrightness);
        printf("\r");
    }
    
    if(beatHold){
        printf("HOLD\r");
    }
    
    if(BUTTONS_isHeld(enter) && !enterPressed){
        beatHold = !beatHold;
    }
    
    if (upState) {
        if(beatBrightness < 9){
            beatBrightness++;
        } else {
            beatBrightness = 9;
        }   
    } else if (downState) {
        if(beatBrightness > 0){
            beatBrightness--;
        } else {
            beatBrightness = 0;
        }   
    }
    
    if(enterState && currentSelection == MODE_BEAT_FADE){
        currentSelection = MODE_BEAT_CONTINUOUS;
    } else if(enterState && currentSelection == MODE_BEAT_STROBE){
        currentSelection = MODE_BEAT_FADE;
    } else if(enterState && currentSelection == MODE_BEAT_CONTINUOUS){
        currentSelection = MODE_BEAT_MIXED;
    } else if(enterState && currentSelection == MODE_BEAT_MIXED){
        currentSelection = MODE_BEAT_STROBE;
    }*/
    
}

bool getIsHold(){
    return false;
}

void static CONTROL_MANUAL(colormode_t input){
    
     if(BUTTONS_isHeld(up) && (CLOCK_getTime() - lastIncTime > incInterval)){
        colorInc(input);
        lastIncTime = CLOCK_getTime();
        if(incInterval > MIN_INTERVAL_TIME_MANUAL){
            incInterval-=INTERVAL_CONST;
        }
    } else if(BUTTONS_isHeld(down) && (CLOCK_getTime() - lastIncTime > incInterval)){
        colorDec(input);
        lastIncTime = CLOCK_getTime();
        if(incInterval > MIN_INTERVAL_TIME_MANUAL){
            incInterval-=INTERVAL_CONST;
        }
    } else if (upState) {
        colorInc(input);
        lastActiveTime = CLOCK_getTime();        
    } else if (downState) {
        colorDec(input);
    } else if(enterState){
        switch(input){
            case CMODE_RED:
                colorModeSelected = CMODE_GREEN;
                break;
                
            case CMODE_GREEN:
                colorModeSelected = CMODE_BLUE;
                break;
                
            case CMODE_BLUE:
                colorModeSelected = CMODE_WHITE;
                break;
                
            case CMODE_WHITE:
                colorModeSelected = CMODE_RED;
                break;
        }
    } else if((!upState || !downState) && !(BUTTONS_isHeld(down) || BUTTONS_isHeld(up))){
        incInterval = MAX_INTERVAL_TIME;
    }
    
}

void static CONTROL_ANIMATION(){

    if(enterState){
        animationBrightnessControl = !animationBrightnessControl;
    }
    
    if (upState && !animationBrightnessControl) {
        if (animationModeSelected < 10) {
            animationModeSelected++;
        } else {
            animationModeSelected = 10;
        }
    } else if (downState && !animationBrightnessControl) {
        if (animationModeSelected > 1) {
            animationModeSelected--;
        } else {
            animationModeSelected = 1;
        }
    } else if (upState && animationBrightnessControl) {
        if (animationBrightness < 10) {
            animationBrightness++;
        } else {
            animationBrightness = 10;
        }
    } else if (downState && animationBrightnessControl) {
        if (animationBrightness > 0) {
            animationBrightness--;
        } else {
            animationBrightness = 0;
        }
    }
    
}