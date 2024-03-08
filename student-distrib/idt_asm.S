#define ASM     1

.globl general_handler

general_handler:
    pushal
    pushfl
    call div_by_0_hdlr
    popfl
    popal
    iret


