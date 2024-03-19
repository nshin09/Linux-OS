
.text
.globl set_PDBR
set_PDBR:
    ; #Setup
    pushl %ebp
    movl %esp, %ebp
    ; #Take arg, save in eax, and move it to CR3
    movl 8(%esp), %eax
    movl %eax, %cr3
    ; #teardown and return
    movl %ebp, %esp
    popl %ebp
    ret


.globl enable_paging
enable_paging:
    ; #Setup
    pushl %ebp
    movl %esp, %ebp
    ; #Set bit 31 of eax to enable paging
    movl %cr0, %eax
    orl $0x8000000, %eax
    movl %eax, %cr0
    ; #teardown and return
    movl %ebp, %esp
    popl %ebp
    ret

