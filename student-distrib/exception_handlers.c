#include "exception_handlers.h"
#include "lib.h"

/* GENERAL HANDLER FUNCTIONS HEADER
 * 
 * void ###_handler();
 * Inputs: None
 * Return Value: None
 * Function: Called when the ### interrupt occurs. Prints
 *           which interrupt occured and deals with the interrupt.
 */

void div_error_handler(){
    printf("Divide by zero error\n");
    while(1);
}

void debug_handler(){
    printf("Debug error \n");
    while(1);
}

void nmi_handler(){
    printf("NMI error \n");
    while(1);
}

void breakpoint_handler(){
    printf("Breakpoint error \n");
    while(1);
}

void overflow_handler(){
    printf("Overflow error \n");
    while(1);
}

void bounds_handler(){
    printf("Bounds error \n");
    while(1);
}

void invalid_opcode_handler(){
    printf("Invalid Opcodes error \n");
    while(1);
}

void dna_handler(){
    printf("Device not available error \n");
    while(1);
}

void double_fault_handler(){
    printf("double fault error \n");
    while(1);
}

void cpo_handler(){
    printf("CPO error \n");
    while(1);
}

void invalid_tss_handler(){
    printf("Invalid TSS error \n");
    while(1);
}

void no_segment_handler(){
    printf("segment not present error \n");
    while(1);
}

void stack_segfault_handler(){
    printf("Stack segfault error \n");
    while(1);
}

void gen_protection_fault_handler(){
    printf("general protection fault error \n");
    while(1);
}

void page_fault_handler(){
    printf("Page fault error \n");
    while(1);
}

void floating_point_handler(){
    printf("Floating point error \n");
    while(1);
}

void alignment_check_handler(){
    printf("Alignment check error \n");
    while(1);
}

void machine_check_handler(){
    printf("Machine check error \n");
    while(1);
}

void simd_floating_point_handler(){
    printf("SIMD floating point error \n");
    while(1);
}

