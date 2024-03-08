
.globl general_handler

general_handler:
    pushal
    pushfl

    popal
    popfl
    iret

