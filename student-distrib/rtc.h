#ifndef _RTC_H
#define _RTC_H

#include "types.h"

// include constant values
#define DEFAULT_FREQUENCY 1024
#define MAX_RATE 0xF
// global variable to be used as a flag if int occurred
volatile int rtc_int;
volatile int rtc_ticks;
int rtc_rate;
int rtc_mode; // indicates if file type is rtc. 
// function that initializes rtc
void rtc_init();
// change the interrupt rate of the rtc
int32_t rtc_change_rate(int32_t frequency);
// opens the rtc
int32_t rtc_open(const uint8_t* filename);
// closes the rtc
int32_t rtc_close(int32_t fd);

/* read from rtc when an interrupt occurs.  */
int32_t rtc_read(int32_t fd, void* buf, int32_t nbytes);
/* write a frequency to rtc  */
int32_t rtc_write(int32_t fd, const void* buf, int32_t nbytes);


#endif
