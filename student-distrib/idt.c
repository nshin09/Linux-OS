#include "x86_desc.h"
#include "idt.h"
#include "lib.h"
#include "idt_asm.h"

//* COMMENT OUT TO TEST BASIC INITIALIZATION
void print_interrupt(){
    printf("Received an Interrupt");
}
//*/

void idt_initializer(){
    printf("Initializing idt - ");

    int i;
    for(i=0; i<256; i++){
        //Only mark the exceptions we deal with as present
        if(i < 32){
            idt[i].present = 1;
            SET_IDT_ENTRY(idt[i], general_handler);
        }
        else{
            idt[i].present = 0;
        }

        //Load the other values of all idt entries
        idt[i].dpl = 0;
        idt[i].seg_selector = 0x0000;
        idt[i].size = 0;
        idt[i].reserved0 = 1;
        idt[i].reserved1 = 1;
        idt[i].reserved2 = 1;
        idt[i].reserved3 = 1;
        idt[i].reserved4 = 1;
    }

    printf("Idt is initialized (i think)\n");
}
