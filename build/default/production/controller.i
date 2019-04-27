# 1 "controller.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "controller.c" 2
# 1 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\stdint.h" 1 3



# 1 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\musl_xc8.h" 1 3
# 4 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\stdint.h" 2 3
# 22 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\stdint.h" 3
# 1 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\bits/alltypes.h" 1 3
# 135 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\bits/alltypes.h" 3
typedef unsigned long uintptr_t;
# 150 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\bits/alltypes.h" 3
typedef long intptr_t;
# 166 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\bits/alltypes.h" 3
typedef signed char int8_t;




typedef short int16_t;




typedef __int24 int24_t;




typedef long int32_t;





typedef long long int64_t;
# 196 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\bits/alltypes.h" 3
typedef long long intmax_t;





typedef unsigned char uint8_t;




typedef unsigned short uint16_t;




typedef __uint24 uint24_t;




typedef unsigned long uint32_t;





typedef unsigned long long uint64_t;
# 237 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\bits/alltypes.h" 3
typedef unsigned long long uintmax_t;
# 22 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\stdint.h" 2 3


typedef int8_t int_fast8_t;

typedef int64_t int_fast64_t;


typedef int8_t int_least8_t;
typedef int16_t int_least16_t;

typedef int24_t int_least24_t;

typedef int32_t int_least32_t;

typedef int64_t int_least64_t;


typedef uint8_t uint_fast8_t;

typedef uint64_t uint_fast64_t;


typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;

typedef uint24_t uint_least24_t;

typedef uint32_t uint_least32_t;

typedef uint64_t uint_least64_t;
# 155 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\stdint.h" 3
# 1 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\bits/stdint.h" 1 3
typedef int32_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef uint32_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
# 155 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\stdint.h" 2 3
# 1 "controller.c" 2

# 1 "C:\\Program Files (x86)\\Microchip\\xc8\\v2.05\\pic\\include\\c99\\stdbool.h" 1 3
# 2 "controller.c" 2

# 1 "./clock.h" 1





typedef uint16_t time_t;

void CLOCK_init();
time_t CLOCK_getTime();
# 3 "controller.c" 2

# 1 "./controller.h" 1








void CONTROLLER_init();
void address_inc();
void address_dec();
void CONTROLLER_task();
uint8_t getAddress();
# 4 "controller.c" 2

# 1 "./buttons.h" 1


typedef enum {
    EVENT_IDLE,
    EVENT_PRESSED,
    EVENT_INTER_HOLD,
    EVENT_HELD
} event_t;

typedef enum {
    STATE_UNPRESSED,
    STATE_PRESSED
} btnState_t;

typedef struct {
    volatile uint8_t* port;
    uint8_t pin;
    btnState_t state;
    btnState_t lastState;
    time_t lastHoldTime;
    event_t event;
} button_t;





void BUTTONS_task();
void BUTTONS_init();
static void init(button_t* btn, volatile uint8_t* port, uint8_t pin);
void static GPIO_init();
_Bool BUTTONS_isClicked(button_t* button);
_Bool BUTTONS_isHeld(button_t* button);
# 5 "controller.c" 2

# 1 "./tm1650.h" 1
# 12 "./tm1650.h"
void TM1650_init();
void TM1650_setDigit(uint8_t digit, uint8_t data);
static void writeData(uint8_t address, uint8_t data);
void TM1650_brightness(uint8_t brightness);
void putch(char n);
void TM1650_scrollPrint(char *array);
void TM1650_fastPrintNum(uint16_t num);
void TM1650_enable(_Bool enable);
_Bool TM1650_isEnabled();
void static welcomeMessage();
# 6 "controller.c" 2


uint16_t address = 1;
button_t *up, *down, *menu, *enter;
int incInterval = 100;
time_t lastIncTime = 0;
time_t lastActiveTime = 0;

void CONTROLLER_init() {
    TM1650_fastPrintNum(address);
}




void address_inc()
{
    if(address == 512)
        address = 1;
    else
        address++;


    TM1650_fastPrintNum(address);
}




void address_dec()
{
    if(address == 1)
        address = 512;
    else
        address--;


    TM1650_fastPrintNum(address);
}

void CONTROLLER_task() {

    _Bool isActive = 1;

    if (BUTTONS_isClicked(up)) {
        address_inc();
        lastActiveTime = CLOCK_getTime();
    } else if (BUTTONS_isClicked(down)) {
        address_dec();
    } else if(BUTTONS_isHeld(up) && (CLOCK_getTime() - lastIncTime > incInterval)){
        address_inc();
        lastIncTime = CLOCK_getTime();
        if(incInterval > 0){
            incInterval-=1;
        }
    } else if(BUTTONS_isHeld(down) && (CLOCK_getTime() - lastIncTime > incInterval)){
        address_dec();
        lastIncTime = CLOCK_getTime();
        if(incInterval > 0){
            incInterval-=1;
        }
    } else if(BUTTONS_isHeld(enter)){
        address = 1;
    } else {
        isActive = 0;
    }

    if(!BUTTONS_isHeld(up) && !BUTTONS_isHeld(down)){
        incInterval = 100;
    }

    if(isActive) {
        lastActiveTime = CLOCK_getTime();
    }

    if (CLOCK_getTime() - lastActiveTime >= 5000) {
        TM1650_enable(0);
        lastActiveTime = CLOCK_getTime() - (5000 + 1);
    } else{
        TM1650_enable(1);
    }


}

uint8_t getAddress(){
    return address;
}
