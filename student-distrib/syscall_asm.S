#define ASM 1

.text

.globl flush_tlb
flush_tlb:
    movl %cr3, %eax
    movl %eax, %cr3
    ret

.globl syscall_handler
syscall_handler:
    pushl %ebp
    pushl %edi
    pushl %esi
    pushl %edx
    pushl %ecx
    pushl %ebx
    #Callee setup

  
    
    #Check eax in valid bounds
    cmpl $1, %eax
    jl FAIL
    cmpl $10, %eax
    jg FAIL

    call *jmptable(,%eax,4)

    #Clear the stack
    #addl $12, %esp

    popl %ebx
    popl %ecx
    popl %edx
    popl %esi
    popl %edi
    popl %ebp


    iret

FAIL:
    popl %ebx
    popl %ecx
    popl %edx
    popl %esi
    popl %edi
    popl %ebp
    movl $-1, %eax
    ; #popfl
    ; #popal
    iret

jmptable:
    .long   halt, halt, execute, read, write, open, close, getargs, vidmap, set_handler, sigreturn 



.globl Save_context
Save_context:
    pushl %eax
    pushl %ebx 
    movl 16(%esp), %eax
    movl 12(%esp), %ebx
    pushl $0x002B 
    pushl %ebx
    pushfl 
    pushl $0x0023               
    pushl %eax   
    movl 20(%esp), %ebx
    movl 24(%esp), %eax
    iret 