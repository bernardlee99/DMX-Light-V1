#ifndef BEAT_H
#define BEAT_H

static void adcThresholdHandler();
void BEAT_init();
bool BEAT_detected();
void putch(char character);
void BEAT_task();

#endif