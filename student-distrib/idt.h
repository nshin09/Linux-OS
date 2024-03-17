#include "types.h"
#ifndef IDT_H
#define IDT_H

extern void idt_initializer();

extern void print_interrupt();
extern void print_exception(uint32_t id);

#endif
