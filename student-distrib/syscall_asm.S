#define ASM 1

.text

.globl flush_tlb, syscall_handler
.globl Save_context

flush_tlb:
    movl %cr3, %eax
    movl %eax, %cr3
    ret

registers:
    .long 0,0,0

syscall_handler:
    # pushl %ebp
    # pushl %edi
    # pushl %esi
    movl %ebp, registers
    movl %edi, registers+4
    movl %esi, registers+8
    pushl %edx
    pushl %ecx
    pushl %ebx
    pushfl
    #Callee setup

  
    
    #Check eax in valid bounds
    cmpl $1, %eax
    jl FAIL
    cmpl $10, %eax
    jg FAIL

    pushl %edx
    pushl %ecx
    pushl %ebx
    call *jmptable(,%eax,4)
    addl $12, %esp
    #Clear the stack

    popfl
    popl %ebx
    popl %ecx
    popl %edx
    movl registers,%ebp
    movl  registers+4,%edi
    movl  registers+8,%esi
    # popl %esi
    # popl %edi
    # popl %ebp


    iret

FAIL:
    popfl
    popl %ebx
    popl %ecx
    popl %edx
    popl %esi
    popl %edi
    popl %ebp
    movl $-1, %eax

    iret

jmptable:
    .long   halt, halt, execute, read, write, open, close, getargs, vidmap, set_handler, sigreturn 

Save_context:
#     pushl %eax
#     pushl %ebx 
    movl 8(%esp), %eax #EIP
    movl 4(%esp), %ebx #ESP
    pushl $0x002B 
    pushl %ebx
    pushfl 
    popl %ebx
    orl $0x200, %ebx
    pushl %ebx
    pushl $0x0023               
    pushl %eax   
    # movl 20(%esp), %ebx
    # movl 24(%esp), %eax
    iret 


# .globl Get_EBP
# Get_EBP:
#     movl %ebp, %eax
#     ret

# Sets EBP and ESP to the passed value correctly.
# ret then gets messed up since the new ESP doesn't have 
# the right EIP value on it's stack.
# .globl Set_EBP_ESP
# Set_EBP_ESP:
#     movl 4(%esp), %ebp
#     movl %ebp, %esp
#     ret 
