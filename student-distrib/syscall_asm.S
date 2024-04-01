#define ASM 1

.text

.globl flush_tlb
flush_tlb:
    movl %cr3, %eax
    movl %eax, %cr3
    ret

.globl syscall_handler
syscall_handler:
    pushal
    pushfl

    ; #Push args
    pushl %edx
    pushl %ecx
    pushl %ebx
    
    ; #Check eax in valid bounds
    cmpl $1, %eax
    jl FAIL
    cmpl $10, %eax
    jg FAIL
    pushl %eax

    call syscall_handler_c
    ; #Clear the stack
    addl $16, %esp

    popl %eax
    popl %ebx
    popl %ecx
    popl %edx
    popfl
    popal
    ret

FAIL:
    popl %ebx
    popl %ecx
    popl %edx
    popfl
    popal
    ret


.globl Save_context
Save_context:
    movl 8(%ebp), %eax
    pushl $0x002B 
    pushl %esp 
    pushfl 
    pushl $0x0010               
    pushl %eax   

    ret 