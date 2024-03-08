//* -Note, this ran just as well on 3/8/24 without #ifndef, #define, and #endif lines
#ifndef IDT_H
#define IDT_H

//prints "received an Interrupt"
extern void print_interrupt();
//*/

//Puts values in all 256 entries of idt
extern void idt_initializer();

#endif
