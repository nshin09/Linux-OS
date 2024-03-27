#define ASM 1

.text

.globl syscall_handler
syscall_handler:
    pushal
    pushfl

    ; #Push args
    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax
    call syscall_handler_c
    ; #Clear the stack
    addl $16, %esp

    popfl
    popal
    ret
