#ifndef stdbool.h
#include<stdbool.h>
#endif

#ifndef stdint.h
#include<stdint.h>
#endif

#ifndef TM1650_H
#define TM1650_H

void TM1650_init();
void TM1650_setDigit(uint8_t digit, uint8_t data);
static void writeData(uint8_t address, uint8_t data);
void TM1650_brightness(uint8_t brightness);
void putch(char n);
void TM1650_scrollPrint(char *array);
void TM1650_fastPrintNum(uint16_t num);
void TM1650_enable(bool enable);
bool TM1650_isEnabled();
void static welcomeMessage();

#endif