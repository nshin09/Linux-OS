#include "lib.h"
#include "i8259.h"

/*
static char Scancodes[] = {' ', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', ' ', ' ', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']',
                    ' ', ' ', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`', ' ', '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/'};
*/

/* void initialize_keyboard();
 * Inputs: None
 * Return Value: None
 * Function: Enables IRQ1 so that the keyboard
 *           interrupts can be received.
 */
void initialize_keyboard(){
    enable_irq(1);
}

/* char findChar(int c);
 * Inputs: c - The keyboard scancode that needs to be
               converted into a character.
 * Return Value: The character equivalent of the keyboard
                 scancode.
 * Function: Converts a keyboard scancode into the
             corresponding character.
 */
char findChar(int c){
    switch(c) {
        case 0x02: return '1';
        case 0x03: return '2';
        case 0x04: return '3';
        case 0x05: return '4';
        case 0x06: return '5';
        case 0x07: return '6';
        case 0x08: return '7';
        case 0x09: return '8';
        case 0x0A: return '9';
        case 0x0B: return '0';
        case 0x0C: return '-';
        case 0x0D: return '=';
        case 0x10: return 'q';
        case 0x11: return 'w';
        case 0x12: return 'e';
        case 0x13: return 'r';
        case 0x14: return 't';
        case 0x15: return 'y';
        case 0x16: return 'u';
        case 0x17: return 'i';
        case 0x18: return 'o';
        case 0x19: return 'p';
        case 0x1A: return '[';
        case 0x1B: return ']';
        case 0x1E: return 'a';
        case 0x1F: return 's';
        case 0x20: return 'd';
        case 0x21: return 'f';
        case 0x22: return 'g';
        case 0x23: return 'h';
        case 0x24: return 'j';
        case 0x25: return 'k';
        case 0x26: return 'l';
        case 0x27: return ';';
        case 0x28: return '\'';
        case 0x29: return '`';
        case 0x2B: return '\\';
        case 0x2C: return 'z';
        case 0x2D: return 'x';
        case 0x2E: return 'c';
        case 0x2F: return 'v';
        case 0x30: return 'b';
        case 0x31: return 'n';
        case 0x32: return 'm';
        case 0x33: return ',';
        case 0x34: return '.';
        case 0x35: return '/';
    }
    return '*';
}

/* void keyboard_handler();
 * Inputs: None
 * Return Value: None
 * Function: Reads a scancode from the keyboard and 
 *           prints the corresponding character on screen.
 *           Also prints the scancode on screen.
 */
void keyboard_handler(){
    cli();
    int Scancode = inb(0x60);
    sti();

    printf("Scancode:%d \n", Scancode);
    char key = findChar(Scancode);
    // putc(key);
    printf("Key:  %c  \n", key);
    /*if(Scancode > strlen(Scancodes) || Scancode < 0){
        return;
    }*/

    send_eoi(0x01); // need to send eoi to irq_1
    
}
