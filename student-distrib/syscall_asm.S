#define ASM 1

.text

.globl flush_tlb, syscall_handler
.globl Save_context

# void flush_tlb();
# Inputs: None
# Return: None
# Function: Flushes the TLB by writing cr3 to itself
flush_tlb:
    movl %cr3, %eax
    movl %eax, %cr3
    ret

registers:
    .long 0,0,0

# void* syscall_handler(uint32_t sycall_num, void* arg1, void* arg2, void* arg3);
# Inputs: syscall_num - the index of the syscall to be called.
#         arg1, arg2, arg3 - arguments to be passed to the syscall. Their functions and existence
#                            vary depending on which syscall is called.
# Return: The return value of the syscall, or -1 if syscall_num is invalid
# Function: Pushes arguments to the stack, checks if syscall_num is valid, and then calls
#           the c function for the corresponding syscall.
syscall_handler:
    #Callee setup

    # pushl %ebp
    # pushl %edi
    # pushl %esi
    pushl %ebp
    pushl %edx
    pushl %ecx
    pushl %ebx
    pushfl
    movl %edi, registers
    movl %esi, registers+4
    
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
    popl %ebp

    movl registers, %edi
    movl registers+4, %esi
    # popl %esi
    # popl %edi
    # popl %ebp

    iret

FAIL:
    popfl
    popl %ebx
    popl %ecx
    popl %edx
    popl %ebp
    movl registers,%edi
    movl registers+4,%esi
    # popl %esi
    # popl %edi
    # popl %ebp
    movl $-1, %eax

    iret

jmptable:
    .long   halt, halt, execute, read, write, open, close, getargs, vidmap, set_handler, sigreturn 

# void Save_context(uint32_t ESP, uint32_t EIP);
# Inputs: ESP - The ESP value to be saved to the stack
#         EIP - The EIP value to be saved to the stack
# Return: None
# Function: Saves ESP and EIP and then switches the prorgams
#           context in order to execute a file.
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

# uint32_t Get_EBP();
# Inputs: None
# Return: The current EBP value
# Function: return EBP so it can be saved in C functions
.globl Get_EBP
Get_EBP:
    movl %ebp, %eax
    ret

# uint32_t Get_ESP();
# Inputs: None
# Return: The current ESP value
# Function: return ESP so it can be saved in C functions
.globl Get_ESP
Get_ESP:
    movl %esp, %eax
    ret

# void Set_EBP_ESP(uint32_t EBP, uint32_t ESP);
# Inputs: EBP - The EBP of the spot to return to
#         ESP - The ESP of the spot to return to
# Return: None
# Function: Resets EBP and ESP to switch back to the original context that 
#           called execute
.globl Set_EBP_ESP
Set_EBP_ESP:
    pushl   %ebp
    movl    %esp, %ebp
    # movl    20(%ebp), %eax  #Save return value
    andl    $0, %eax
    movb    %bl, %al
    movl	12(%ebp), %esp
    movl    8(%ebp), %ebp
    leave
    ret 

.end
