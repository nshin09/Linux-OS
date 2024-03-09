
.globl general_handler

general_handler:
    pushal
    pushfl

    call print_interrupt

    popfl
    popal
    iret     
    # Making this RET instead of IRET lets this function be called from 
    # kernel.c (~line 153). Enabling interrupts still bootloops with that.

