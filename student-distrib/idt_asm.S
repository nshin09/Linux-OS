
.globl general_handler

general_handler:
    pushal
    pushfl

    popfl
    popal
    iret

