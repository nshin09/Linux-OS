#ifndef _DEVICE_HANDLERS_H
#define _DEVICE_HANDLERS_H

#include "rtc.h"

//Handler for rtc interrupts
void RTC_HANDLER();
//Handler for keyboard interrupts
void KEYBOARD_HANDLER();
void PIT_HANDLER();
#endif
