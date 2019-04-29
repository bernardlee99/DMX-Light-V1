#include "clock.h"
#include "controller.h"
#include "buttons.h"
#include "tm1650.h"
#include "dmx.h"
#include "led.h"

typedef void (*fn_t)(void);


typedef struct {
    menuItem_t* nextItem;
    menuItem_t* prevItem;
    char name[4];
    fn_t config;
} menuItem_t;

menuItem_t currentMenu;

void dmxAddressPrint() {
    
}

    menuItem_t dmx;
    menuItem_t beat;
    menuItem_t animation;
    menuItem_t manual;
    
      // beat configs
    menuItem_t beatStrobe;
    menuItem_t beatFade;
    menuItem_t beatCont;
    menuItem_t beatMixed;

void beatConfig() {
    setMenu(&beatStrobe);
}


    
void CONTROLLER_init() {

    dmx.nextItem = &beat;
    beat.nextItem = &animation;
    animation.nextItem = &manual;
    
    dmx.config = dmxAddressPrint;
    
    beat.config = beatConfig;
    

    
    beatStrobe.nextItem = &beatFade;
    
    // animation configs
    menuItem_t animationType;
    
    // manualConfigs;
    menuItem_t manualRed;
    
    dmx.nextItem = &beatStrobe;
    beat
}

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

bool beatHold = false;

colormode_t colorModeSelected = CMODE_RED;

uint8_t animationModeSelected = 1;
bool animationBrightnessControl = false;
uint8_t animationBrightness;

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
    
}

void menuSelection(){
    
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
    
    if(BUTTONS_isHeld(enter) && !enterPressed){
        beatHold = !beatHold;
        enterPressed = true;
        return true;
    }
    
    //beatHold = false;
    
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
        currentSelection = MODE_BEAT_CONTINUOUS;
        //TM1650_scrollPrint("FLASH\r");
    } else if(enterState && currentSelection == MODE_BEAT_STROBE){
        currentSelection = MODE_BEAT_FADE;
    } else if(enterState && currentSelection == MODE_BEAT_CONTINUOUS){
        currentSelection = MODE_BEAT_MIXED;
    } else if(enterState && currentSelection == MODE_BEAT_MIXED){
        currentSelection = MODE_BEAT_STROBE;
    }
    
}

bool getIsHold(){
    return false;
}

bool static CONTROL_MANUAL(colormode_t input){
    
     if(BUTTONS_isHeld(up) && (CLOCK_getTime() - lastIncTime > incInterval)){
        colorInc(input);
        lastIncTime = CLOCK_getTime();
        if(incInterval > MIN_INTERVAL_TIME_MANUAL){
            incInterval-=INTERVAL_CONST;
        }
        return true;
    } else if(BUTTONS_isHeld(down) && (CLOCK_getTime() - lastIncTime > incInterval)){
        colorDec(input);
        lastIncTime = CLOCK_getTime();
        if(incInterval > MIN_INTERVAL_TIME_MANUAL){
            incInterval-=INTERVAL_CONST;
        }
        return true;
    } else if (upState) {
        colorInc(input);
        return true;
        lastActiveTime = CLOCK_getTime();        
    } else if (downState) {
        colorDec(input);
        return true;
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
        return true;
    } else if((!upState || !downState) && !(BUTTONS_isHeld(down) || BUTTONS_isHeld(up))){
        incInterval = MAX_INTERVAL_TIME;
    } else {
        return false;
    }
    
    return false;
    
}

bool static CONTROL_ANIMATION(){

    if(enterState){
        !animationBrightnessControl;
        return true;
    }
    
    if (upState && !animationBrightnessControl) {
        if (animationModeSelected < 10) {
            animationModeSelected++;
        } else {
            animationModeSelected = 10;
        }
        return true;
    } else if (downState && !animationBrightnessControl) {
        if (animationModeSelected > 1) {
            animationModeSelected--;
        } else {
            animationModeSelected = 1;
        }
        return true;
    } else if (upState && animationBrightnessControl) {
        if (animationBrightness < 10) {
            animationBrightness++;
        } else {
            animationBrightness = 10;
        }
        return true;
    } else if (downState && animationBrightnessControl) {
        if (animationBrightness > 0) {
            animationBrightness--;
        } else {
            animationBrightness = 0;
        }
        return true;
    }
    
}