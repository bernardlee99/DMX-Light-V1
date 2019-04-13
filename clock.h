#ifndef CLOCK_H
#define	CLOCK_H

#include <stdint.h>
    
typedef uint16_t time_t;

void CLOCK_init();
time_t CLOCK_getTime();

#endif	/* CLOCK_H */