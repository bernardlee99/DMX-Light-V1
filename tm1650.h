void TM1650_init();
void TM1650_setDigit(uint8_t digit, uint8_t data);
static void writeData(uint8_t address, uint8_t data);
void TM1650_brightness(uint8_t brightness);
void putch(char n);
void scrollPrint(char *array);

