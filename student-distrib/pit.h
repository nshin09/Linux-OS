#ifndef _PIT_H
#define _PIT_H
#include "types.h"

#define HZ 100
#define RATE 1193180
#define DEFAULT_TICKS 18
extern void pit_init();
extern void pit_handler();
#endif
