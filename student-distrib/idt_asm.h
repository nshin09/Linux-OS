#ifndef IDT_ASM_H
#define IDT_ASM_H

void general_handler();
void Division_Error();
void Debug();
void Non_maskable_Interrupt();
void Breakpoint();
void Overflow();
void Bound_Range_Exceeded();
void Invalid_Opcode();
void Device_Not_Available();
void Double_Fault();
void Coprocessor_Segment_Overrun();
void Invalid_TSS();
void Segment_Not_Present();
void Stack_Segment_Fault();
void General_Protection_Fault();
void Page_Fault();
void Floating_Point_Exception();
void Alignment_Check();
void Machine_Check();
void SIMD_Floating_Point_Exception();
// void Virtualization_Exception();
// void Control_Protection_Exception();
// void Hypervisor_Injection_Exception();
// void VMM_Communication_Exception();
// void Security_Exception();

#endif
