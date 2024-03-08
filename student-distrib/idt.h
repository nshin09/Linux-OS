<<<<<<< HEAD
#ifndef _IDT_H
#define _IDT_H

#include "types.h"
#include "x86_desc.h"

#ifndef ASM

extern void div_by_0_hdlr();

extern void idt_initializer();

extern void handler(int vec);

#endif
#endif /* _IDT_H */
=======
//* -Note, this ran just as well on 3/8/24 without #ifndef, #define, and #endif lines
#ifndef IDT_H
#define IDT_H

//prints "received an Interrupt"
extern void print_interrupt();
//*/

//Puts values in all 256 entries of idt
extern void idt_initializer();

#endif
>>>>>>> idt_testing_branch
