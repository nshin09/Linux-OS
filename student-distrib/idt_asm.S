.globl Division_Error, Debug, Non_maskable_Interrupt, Breakpoint, Overflow
.globl Bound_Range_Exceeded, Invalid_Opcode, Device_Not_Available, Double_Fault
.globl Coprocessor_Segment_Overrun, Invalid_TSS, Segment_Not_Present, Stack_Segment_Fault
.globl General_Protection_Fault, Page_Fault, Floating_Point_Exception, Alignment_Check
.globl Machine_Check, SIMD_Floating_Point_Exception
; .globl Control_Protection_Exception, Hypervisor_Injection_Exception,VMM_Communication_Exception, Security_Exception
 
; /*
;  *These first functions save all flags and refisters and then jump to handlers lower in this file.
;  s*/
/* void Exception Handlers();
 * Inputs: None
 * Return Value: None
 * Function: Prepares arguments to be called in respective handler.
 */
.align 4
Division_Error: 
    pushl $0 
    pushal 
    pushfl
    jmp Division_Error_Handler

.align 4
Debug:
    pushl $1
    pushal 
    pushfl 
    jmp Debug_Handler

.align 4
Non_maskable_Interrupt:
    pushl $2
    pushal 
    pushfl 
    jmp NMI_Handler

.align 4
Breakpoint:
    pushl $3
    pushal 
    pushfl 
    jmp Breakpoint_Handler

.align 4
Overflow:
    pushl $4
    pushal 
    pushfl 
    jmp Overflow_Handler

.align 4
Bound_Range_Exceeded:
    pushl $5
    pushal 
    pushfl 
    jmp Bounds_Handler

.align 4
Invalid_Opcode:
    pushl $6
    pushal 
    pushfl 
    jmp Invalid_Opcode_Handler

.align 4
Device_Not_Available:
    pushl $7
    pushal 
    pushfl 
    jmp Device_Not_Available_Handler

.align 4
Double_Fault:
    pushl $8
    pushal 
    pushfl 
    jmp Double_Fault_Handler

.align 4
Coprocessor_Segment_Overrun:
    pushl $9
    pushal 
    pushfl 
    jmp Coprocessor_Segment_Overrun_Handler

.align 4
Invalid_TSS:
    pushl $10
    pushal 
    pushfl 
    jmp Invalid_TSS_Handler

.align 4
Segment_Not_Present:
    pushl $11
    pushal 
    pushfl 
    jmp Segment_Not_Present_Handler

.align 4
Stack_Segment_Fault:
    pushl $12
    pushal 
    pushfl 
    jmp Stack_Segment_Fault_Handler

.align 4
General_Protection_Fault: 
    pushl $13
    pushal 
    pushfl
    jmp General_Protection_Fault_Handler

.align 4
Page_Fault:
    pushl $14
    pushal 
    pushfl 
    jmp Page_Fault_Handler

.align 4
Floating_Point_Exception:
    pushl $16
    pushal 
    pushfl 
    jmp Floating_Point_Handler

.align 4
Alignment_Check:
    pushl $17
    pushal 
    pushfl 
    jmp Alignment_Check_Handler

.align 4
Machine_Check:
    pushl $18
    pushal 
    pushfl 
    jmp Machine_Check_Handler

.align 4
SIMD_Floating_Point_Exception:
    pushl $19
    pushal 
    pushfl 
    jmp SIMD_Floating_Point_Handler

 /*
;  *These first functions save all flags and refisters and then jump to handlers lower in this file.
;  s*/
/* void Exception Handlers();
 * Inputs: None
 * Return Value: None
 * Function: Prepares arguments to be called in respective handler.
 */
Division_Error_Handler:
    call div_error_handler /* Change this to print exception once we get exception to work. */
    popfl
    popal
    add $4, %esp 
   
    iret     

Debug_Handler:
    call debug_handler /* Change this to print exception once we get exception to work. */
    popfl
    popal
    add $4, %esp 
   
    iret     

NMI_Handler:
    ; pushal
    ; pushfl /* Comment this line and the one above it out once we get int handler to work. */
    call nmi_handler /* Change this to print exception once we get exception to work. */
    popfl
    popal
    add $4, %esp 
   
    iret     

Breakpoint_Handler:
    call breakpoint_handler /* Change this to print exception once we get exception to work. */
    popfl
    popal
    add $4, %esp 
   
    iret     


Overflow_Handler:
    call overflow_handler /* Change this to print exception once we get exception to work. */
    popfl
    popal
    add $4, %esp 
   
    iret     

Bounds_Handler:
    call bounds_handler /* Change this to print exception once we get exception to work. */
    popfl
    popal
    add $4, %esp 
   
    iret     

Invalid_Opcode_Handler:
    call invalid_opcode_handler /* Change this to print exception once we get exception to work. */
    popfl
    popal
    add $4, %esp 
   
    iret     

Device_Not_Available_Handler:
    call dna_handler /* Change this to print exception once we get exception to work. */
    popfl
    popal
    add $4, %esp 
   
    iret     

Double_Fault_Handler:
    call double_fault_handler /* Change this to print exception once we get exception to work. */
    popfl
    popal
    add $4, %esp 
   
    iret     

Coprocessor_Segment_Overrun_Handler:
    call cpo_handler /* Change this to print exception once we get exception to work. */
    popfl
    popal
    add $4, %esp 
   
    iret     

Invalid_TSS_Handler:
    call invalid_tss_handler /* Change this to print exception once we get exception to work. */
    popfl
    popal
    add $4, %esp 
   
    iret     

Segment_Not_Present_Handler:
    call no_segment_handler /* Change this to print exception once we get exception to work. */
    popfl
    popal
    add $4, %esp 
   
    iret     

Stack_Segment_Fault_Handler:
    call stack_segfault_handler /* Change this to print exception once we get exception to work. */
    popfl
    popal
    add $4, %esp 
   
    iret     

General_Protection_Fault_Handler:
    ; pushal
    ; pushfl /* Comment this line and the one above it out once we get int handler to work. */
    call gen_protection_fault_handler /* Change this to print exception once we get exception to work. */
    popfl
    popal
    add $4, %esp 
   
    iret     

Page_Fault_Handler:
    call page_fault_handler /* Change this to print exception once we get exception to work. */
    popfl
    popal
    add $4, %esp 
   
    iret     

Floating_Point_Handler:
    call floating_point_handler /* Change this to print exception once we get exception to work. */
    popfl
    popal
    add $4, %esp 
   
    iret     

Alignment_Check_Handler:
    call alignment_check_handler /* Change this to print exception once we get exception to work. */
    popfl
    popal
    add $4, %esp 
   
    iret     

Machine_Check_Handler:
    call machine_check_handler /* Change this to print exception once we get exception to work. */
    popfl
    popal
    add $4, %esp 
   
    iret     

SIMD_Floating_Point_Handler:
    call simd_floating_point_handler /* Change this to print exception once we get exception to work. */
    popfl
    popal
    add $4, %esp 
   
    iret     
