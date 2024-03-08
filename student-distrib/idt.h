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
