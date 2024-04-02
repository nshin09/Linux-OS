#define ASM 1

.text

.globl flush_tlb
flush_tlb:
    movl %cr3, %eax
    movl %eax, %cr3
    ret

.globl syscall_handler
syscall_handler:
    ;pushal
    ;pushfl

    ; #Push args
    pushl %edx
    pushl %ecx
    pushl %ebx
    
    ; #Check eax in valid bounds
    cmpl $1, %eax
    jl FAIL
    cmpl $10, %eax
    jg FAIL

    call *jmptable(,%eax,4)

    ; #call syscall_handler_c
    ; #Clear the stack
    ; #addl $12, %esp

    popl %ebx
    popl %ecx
    popl %edx
    ; #popfl
    ; #popal
    leave
    ret

FAIL:
    popl %ebx
    popl %ecx
    popl %edx
    ; #popfl
    ; #popal
    leave
    ret

jmptable:
    .long   syscall_handler_c, halt, execute, read
    .long   write, open, close, getargs
    .long   vidmap, set_handler, sigreturn 



.globl Save_context
Save_context:
    movl 8(%esp), %eax
    movl 4(%esp), %ebx
    pushl $0x002B 
    pushl %ebx
    pushfl 
    pushl $0x0023               
    pushl %eax   

    iret 