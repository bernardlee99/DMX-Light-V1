#ifndef STD_H
#define STD_H
#include<stdbool.h>
#include<stdint.h>
#include <xc.h>
#endif

#ifndef CLOCK_H
#define	CLOCK_H

#include <stdint.h>
    
typedef uint16_t time_t;

void CLOCK_init();
time_t CLOCK_getTime();
extern time_t startTime;

#endif	/* CLOCK_H */