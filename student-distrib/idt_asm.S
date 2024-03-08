<<<<<<< HEAD
#define ASM     1
=======
>>>>>>> idt_testing_branch

.globl general_handler

general_handler:
    pushal
    pushfl
<<<<<<< HEAD
    call div_by_0_hdlr
    popfl
    popal
    iret


=======

    popal
    popfl
    iret

>>>>>>> idt_testing_branch
