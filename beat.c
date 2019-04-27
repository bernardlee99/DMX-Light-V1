/*
 * File:   beat.c
 * Author: student
 *
 * Created on April 26, 2019, 5:47 PM
 */


#include "beat.h"
#include "mcc_generated_files/adcc.h"
#include "clock.h"
#include "tm1650.h"
#include "stdio.h"

// private defines
#define MIN_TEMPO               80
#define MAX_TEMPO               160
#define DELAY_WAITING_FOR_BEAT  20
#define DELAY_BEAT_FOUND        100

// private methods
static void adcThresholdHandler();

// private members
static time_t lastBeat;                // when was the last beat?
static uint16_t bpm = 0;               // last BPM
static bool detected = false;          // whether a beat was detected
static bool thresholdExceeded = false; // whether threshold was exceeded
static time_t lastRun = 0;             // the last time the function ran
static uint16_t threshold = 1680;      // threshold for ADC
static uint16_t delay = 10;            // delay time for task
static int thresholdCount = 0;         // number of continuous threshold events
static bool lastDetected = 0;          // whether the last run detected a beat

static void adcThresholdHandler() {
    thresholdExceeded = true;
}

void BEAT_init() {
    ADCC_SetUpperThreshold(threshold);
    ADCC_SetADTIInterruptHandler(adcThresholdHandler);
    ADCC_StartConversion(0x0F); // start conversion on PCH = RB7/ANB7
}

/**
 * Returns whether a beat was detected since the last time this method was called
 * @return whether a beat was detected
 */
bool BEAT_detected() {
    if (detected) {
        detected = false;
        return true;
    }

    return false;
}

// redirect printf() to the UART. Comment this out
// to use a TM1650 putch())
/*void putch(char character) {
    while (!TX1STAbits.TRMT);
    TXREG = character;
}*/

void BEAT_task() {
    // don't bother doing anything if the display is on

    if (CLOCK_getTime() - lastRun < delay)
        return;

    lastRun = CLOCK_getTime();

    if (thresholdExceeded == true) {
        // we exceeded the threshold in the last sampling window
        // keep track of clipping by counting the number of 
        // times we've had a threshold event in a row
        thresholdCount++;
    } else {
        // we didn't exceed the threshold in the last sampling window
        thresholdCount = 0;
        delay = DELAY_WAITING_FOR_BEAT;
    }

    // Look for an edge. An edge is defined as a threshold event
    // when the previous time we ran didn't produce a threshold
    // event. A.k.a., it went from "quiet" to "loud"
    if (thresholdExceeded == true && lastDetected == false) {
        // we found an edge, so let our event know it can return true
        detected = true;
        // wait for a bit before we run the task again. There's no
        // sense in looking for a beat for at least another 100 ms.
        delay = DELAY_BEAT_FOUND;

        // calculate the current bpm based on the last beat
        time_t offset = CLOCK_getTime() - lastBeat;

        lastBeat = CLOCK_getTime();

        if (offset < 150) // clip at 400 bpm
            bpm = 400;
        else
            bpm = 60 * 1000 / offset;

        // uncomment this to get print messages when a beat happens
        // printf("Beat! Estimated BPM = %u\r\n", bpm);
    }

    lastDetected = thresholdExceeded;
    thresholdExceeded = false;

    if (thresholdCount > 3) {
        threshold += 1;
        ADCC_SetUpperThreshold(threshold);
        thresholdCount = 0;
        // uncomment this to see when thresholds are adjusted
        // printf("Inc. Thresh = %u\r\n", threshold);
        lastBeat = CLOCK_getTime();
    } else if (CLOCK_getTime() - lastBeat > 1000) {
        threshold -= 1;
        ADCC_SetUpperThreshold(threshold);
        // uncomment this to see when thresholds are adjusted
        // printf("Dec. Thresh = %u\r\n", threshold);
    }
}
