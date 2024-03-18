.globl Division_Error, Debug, Non_maskable_Interrupt, Breakpoint, Overflow
.globl Bound_Range_Exceeded, Invalid_Opcode, Device_Not_Available, Double_Fault
.globl Coprocessor_Segment_Overrun, Invalid_TSS, Segment_Not_Present, Stack_Segment_Fault
.globl General_Protection_Fault, Page_Fault, Floating_Point_Exception, Alignment_Check
.globl Machine_Check, SIMD_Floating_Point_Exception, Virtualization_Exception
; .globl Control_Protection_Exception, Hypervisor_Injection_Exception,VMM_Communication_Exception, Security_Exception
 
 
.align 4
Division_Error: 
    pushal 
    pushfl
    pushl $0 
    jmp general_handler

.align 4
Debug:
    pushl $1
    pushal 
    pushfl 
    jmp general_handler

.align 4
Non_maskable_Interrupt:
    pushl $2
    pushal 
    pushfl 
    jmp general_handler

.align 4
Breakpoint:
    pushl $3
    pushal 
    pushfl 
    jmp general_handler

.align 4
Overflow:
    pushl $4
    pushal 
    pushfl 
    jmp general_handler

.align 4
Bound_Range_Exceeded:
    pushl $5
    pushal 
    pushfl 
    jmp general_handler

.align 4
Invalid_Opcode:
    pushl $6
    pushal 
    pushfl 
    jmp general_handler

.align 4
Device_Not_Available:
    pushl $7
    pushal 
    pushfl 
    jmp general_handler

.align 4
Double_Fault:
    pushl $8
    pushal 
    pushfl 
    jmp general_handler

.align 4
Coprocessor_Segment_Overrun:
    pushl $9
    pushal 
    pushfl 
    jmp general_handler

.align 4
Invalid_TSS:
    pushl $10
    pushal 
    pushfl 
    jmp general_handler

.align 4
Segment_Not_Present:
    pushl $11
    pushal 
    pushfl 
    jmp general_handler

.align 4
Stack_Segment_Fault:
    pushl $12
    pushal 
    pushfl 
    jmp general_handler

.align 4
General_Protection_Fault: 
    pushl $13
    pushal 
    pushfl
    jmp general_handler

.align 4
Page_Fault:
    pushl $14
    pushal 
    pushfl 
    jmp general_handler

.align 4
Floating_Point_Exception:
    pushl $16
    pushal 
    pushfl 
    jmp general_handler

.align 4
Alignment_Check:
    pushl $17
    pushal 
    pushfl 
    jmp general_handler

.align 4
Machine_Check:
    pushl $18
    pushal 
    pushfl 
    jmp general_handler

.align 4
SIMD_Floating_Point_Exception:
    pushl $19
    pushal 
    pushfl 
    jmp general_handler

.align 4
Virtualization_Exception:
    pushl $20
    pushal 
    pushfl 
    jmp general_handler

; .align 4
; Control_Protection_Exception:
;     pushl $21
;     pushal 
;     pushfl 
;     jmp general_handler

; .align 4
; Hypervisor_Injection_Exception:
;     pushl $28
;     pushal 
;     pushfl 
;     jmp general_handler

; .align 4
; VMM_Communication_Exception:
;     pushl $29
;     pushal 
;     pushfl 
;     jmp general_handler

; .align 4
; Security_Exception:
;     pushl $30
;     pushal 
;     pushfl 
;     jmp general_handler



general_handler:
    ; pushal
    ; pushfl /* Comment this line and the one above it out once we get int handler to work. */
    call print_exception /* Change this to print exception once we get exception to work. */
    popfl
    popal
    add $4, %esp 
   
    iret     
    # Making this RET instead of IRET lets this function be called from 
    # kernel.c (~line 153). Enabling interrupts still bootloops with that.

