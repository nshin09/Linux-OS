#define ASM 1

/* void set_PDBR();
 * Inputs: pagedirectory pointer
 * Return Value: None
 * Function: moves the pointer to the cr3 reg
 */
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
    ;# movl %ebp, %esp
    ;# popl %ebp
    leave
    ret

/* void enable_paging();
 * Inputs: None
 * Return Value: None
 * Function: performs bit or routines to cr4 and cr0
 */
.globl enable_paging
enable_paging:
    ; #Setup
    pushl %ebp
    movl %esp, %ebp



    ; #Set bit 31 of eax to enable paging
    movl %cr4, %eax
    orl $0x00000010, %eax
    movl %eax, %cr4

    movl %cr0, %eax
    orl $0x80000000, %eax
    movl %eax, %cr0

  

    ; #teardown and return
    ;# movl %ebp, %esp
    ; #popl %ebp
    leave
    ret

