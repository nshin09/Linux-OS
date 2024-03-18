#include "lib.h"
#include "i8259.h"

static char Scancodes[] = {' ', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', ' ', ' ', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']',
                    ' ', ' ', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`', ' ', '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/'};

void initialize_keyboard(){
    enable_irq(1);
}

void keyboard_handler(){
    cli();
    int Scancode = inb(0x60);
    sti();

    printf("%d \n", Scancode);
    /*if(Scancode > strlen(Scancodes) || Scancode < 0){
        return;
    }*/

    printf("Got a keyboard interrupt"); //Scancodes[Scancode]);
    send_eoi(0x01);
    
}
