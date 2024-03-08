#include "idt.h"
#include "lib.h"
#include "x86_desc.h"
#include "idt_asm.h"


void div_by_0_hdlr(){
    //handles div by 0 stuff
    printf("AHHHH");
}

void handler(int vec){
    // switch(vec){
    //     case 0:
    //         printf(vec);
    //         break;
    //     case 1:
    //         printf(vec);
    //         break;
    //     case 2:
    //         printf(vec);
    //         break;
    //     case 3:
    //         printf(vec);
    //         break;
    //     case 4:
    //         printf(vec);
    //         break;
    //     case 5:
    //         printf(vec);
    //         break;
    //     case 6:
    //         printf(vec);
    //         break;
    //     case 7:
    //         printf(vec);
    //         break;
    //     case 8:
    //         printf(vec);
    //         break;
    //     case 9:
    //         printf(vec);
    //         break;
    //     case 10:
    //         printf(vec);
    //         break;
    //     case 11:
    //         printf(vec);
    //         break;
    //     case 12:
    //         printf(vec);
    //         break;
    //     case 13:
    //         printf(vec);
    //         break;
    //     case 14:
    //         printf(vec);
    //         break;
    //     case 15:
    //         printf(vec);
    //         break;
    //     case 16:
    //         printf(vec);
    //         break;
    //     case 17:
    //         printf(vec);
    //         break;
    //     case 18:
    //         printf(vec);
    //         break;
    //     case 19:
    //         printf(vec);
    //         break;
        
    // }

}

void idt_initializer(){
    int i;
    for(i = 0; i < 256; i++){
        if( i < 32){
            //SET_IDT_ENTRY takes (idt_desc_t, int?)
            general_handler();
            // SET_IDT_ENTRY(idt[i], general_handler);
            idt[i].present = 1;
        }
        else{
            
            idt[i].present = 0;
        }
        idt[i].dpl = 0;
        idt[i].seg_selector = 0x0000;
        idt[i].size = 0;
        idt[i].reserved4 = 1; 
        idt[i].reserved3 = 1;
        idt[i].reserved2 = 1; 
        idt[i].reserved1 = 1;
        idt[i].reserved1 = 0;
    }
}

/*
 * Loop through first 32 entries in IDT
 *      Call SET_IDT_ENTRY(???) for each entry
 *                      ??? = MY_ASM_MACRO? or function_name from idt.S?
 * 
 * //load idt
 * lidt idt_desc_t     
 */



// void execute_hdlr(int vector){
//     if(vector == 0){
//         printf("div by 0")
//     }
// }


