#include "types.h"
#ifndef EXCEPTION_HANDLERS_H
#define EXCEPTION_HANDLERS_H

// Interrupt handlers. Each handler deals with it's corresponding interrupt
extern void div_error_handler();

extern void debug_handler();

extern void nmi_handler();

extern void breakpoint_handler();

extern void overflow_handler();

extern void bounds_handler();

extern void invalid_opcode_handler();

extern void dna_handler();

extern void double_fault_handler();

extern void cpo_handler();

extern void invalid_tss_handler();

extern void no_segment_handler();

extern void stack_segfault_handler();

extern void gen_protection_fault_handler();

extern void page_fault_handler();

extern void floating_point_handler();

extern void alignment_check_handler();

extern void machine_check_handler();

extern void simd_floating_point_handler();


#endif