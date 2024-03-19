#include "idt.h"
#include "x86_desc.h"
#include "lib.h"
#include "idt_asm.h"
#include "device_handlers.h"

/* Commented out to avoid unused variable warning
static char* exceptions[32] = 
{
"Division_Error", "Debug", "Non_maskable_Interrupt", "Breakpoint", "Overflow"
"Bound_Range_Exceeded", "Invalid_Opcode", "Device_Not_Available", "Double_Fault"
 "Coprocessor_Segment_Overrun", "Invalid_TSS", "Segment_Not_Present", "Stack_Segment_Fault"
"General_Protection_Fault", "Page_Fault", "Reserved", "Floating_Point_Exception", "Alignment_Check"
"Machine_Check", "SIMD_Floating_Point_Exception", "Virtualization_Exception"
"Control_Protection_Exception","Reserved","Reserved","Reserved",
"Reserved","Reserved","Reserved", "Hypervisor_Injection_Exception","VMM_Communication_Exception", "Security_Exception","Reserved"
};
//*/


/* void idt_initializer();
 * Inputs: None
 * Return Value: None
 * Function: Initializes the values within the idt table
 *           so that the first 20 interrupts, as well as
 *           RTC and keyboard interrupts, can be used.
 */
void idt_initializer(){
    int i;
    for(i=0; i<20; i++){
        //Set the values of all interrupts we deal with

        if(i == 15)
        {
            //Skip interrupt 15 as its reserved
            continue;
        }
        
        idt[i].present = 1;
        idt[i].dpl = 0;  //protection level
        idt[i].seg_selector = KERNEL_CS;
        idt[i].size = 1;
        idt[i].reserved0 = 0;
        idt[i].reserved1 = 1;
        idt[i].reserved2 = 1;
        idt[i].reserved3 = 1;
        idt[i].reserved4 = 0;


    }
        // comment these back in once we get exception to work. 
        SET_IDT_ENTRY(idt[0], Division_Error);

        SET_IDT_ENTRY(idt[1], Debug);
        SET_IDT_ENTRY(idt[2], Non_maskable_Interrupt);
        SET_IDT_ENTRY(idt[3], Breakpoint);
        SET_IDT_ENTRY(idt[4], Overflow);
        SET_IDT_ENTRY(idt[5], Bound_Range_Exceeded);
        SET_IDT_ENTRY(idt[6], Invalid_Opcode);
        SET_IDT_ENTRY(idt[7],  Device_Not_Available);
        SET_IDT_ENTRY(idt[8], Double_Fault);
        SET_IDT_ENTRY(idt[9], Coprocessor_Segment_Overrun);
        SET_IDT_ENTRY(idt[10], Invalid_TSS);
        SET_IDT_ENTRY(idt[11],  Segment_Not_Present);
        SET_IDT_ENTRY(idt[12], Stack_Segment_Fault);
        SET_IDT_ENTRY(idt[13], General_Protection_Fault);
        SET_IDT_ENTRY(idt[14], Page_Fault);
        SET_IDT_ENTRY(idt[16], Floating_Point_Exception);
        SET_IDT_ENTRY(idt[17],  Alignment_Check);
        SET_IDT_ENTRY(idt[18], Machine_Check);
        SET_IDT_ENTRY(idt[19],  SIMD_Floating_Point_Exception);
        // SET_IDT_ENTRY(idt[20],  Virtualization_Exception);
        // SET_IDT_ENTRY(idt[21],  Control_Protection_Exception);
        // SET_IDT_ENTRY(idt[28],  Hypervisor_Injection_Exception);
        // SET_IDT_ENTRY(idt[29],  VMM_Communication_Exception);
        // SET_IDT_ENTRY(idt[30],  Security_Exception);

    // Set the RTC and keyboard interrupt values. These have
    // different reserved bits because they're different interrupt types
    idt[0x28].present = 1;
    idt[0x28].reserved0 = 0;  
    idt[0x28].reserved1 = 1;
    idt[0x28].reserved2 = 1;
    idt[0x28].reserved3 = 0;
    idt[0x28].reserved4 = 0;
    idt[0x28].size = 1;
    idt[0x28].seg_selector = KERNEL_CS;
    idt[0x28].dpl = 0; // 0x28 represents the RTC port described in the IDT 
    printf("before setting idt entry \n");
    SET_IDT_ENTRY(idt[0x28], RTC_HANDLER);
    
    idt[0x21].present = 1;
    idt[0x21].reserved0 = 0; 
    idt[0x21].reserved1 = 1;
    idt[0x21].reserved2 = 1;
    idt[0x21].reserved3 = 0;
    idt[0x21].reserved4 = 0;
    idt[0x21].size = 1;
    idt[0x21].seg_selector = KERNEL_CS;
    idt[0x21].dpl = 0;
    SET_IDT_ENTRY(idt[0x21], KEYBOARD_HANDLER);


}

