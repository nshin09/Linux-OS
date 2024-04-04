#include "lib.h"
#include "i8259.h"
#include "keyboard.h"
#include "terminal.h"


/* void initialize_keyboard();
 * Inputs: None
 * Return Value: None
 * Function: Enables IRQ1 so that the keyboard
 *           interrupts can be received.
 */
void initialize_keyboard(){
    enable_irq(1);
    keyboard_buffer_index = 0;
    caps_lock = 0;
    shift = 0;
    ctrl = 0;
    alt = 0;
    textOverflow = 0;
    gotNewLine = 0;
   
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
        case 0x39: return ' ';
        default: return '\0';
    }
    return '\0';
}

/* char findShiftedChar(int c);
 * Inputs: c - The keyboard scancode that needs to be
               converted into a shifted character.
 * Return Value: The character equivalent of the keyboard
                 scancode assuming shift is pressed.
 * Function: Converts a keyboard scancode into either the
             capital version of that key or the secondary symbol
             on that key.
 */
char findShiftedChar(int c){
    switch(c) {
        case 0x02: return '!';
        case 0x03: return '@';
        case 0x04: return '#';
        case 0x05: return '$';
        case 0x06: return '%';
        case 0x07: return '^';
        case 0x08: return '&';
        case 0x09: return '*';
        case 0x0A: return '(';
        case 0x0B: return ')';
        case 0x0C: return '_';
        case 0x0D: return '+';
        case 0x10: return 'Q';
        case 0x11: return 'W';
        case 0x12: return 'E';
        case 0x13: return 'R';
        case 0x14: return 'T';
        case 0x15: return 'Y';
        case 0x16: return 'U';
        case 0x17: return 'I';
        case 0x18: return 'O';
        case 0x19: return 'P';
        case 0x1A: return '{';
        case 0x1B: return '}';
        case 0x1E: return 'A';
        case 0x1F: return 'S';
        case 0x20: return 'D';
        case 0x21: return 'F';
        case 0x22: return 'G';
        case 0x23: return 'H';
        case 0x24: return 'J';
        case 0x25: return 'K';
        case 0x26: return 'L';
        case 0x27: return ':';
        case 0x28: return '\"';
        case 0x29: return '~';
        case 0x2B: return '|';
        case 0x2C: return 'Z';
        case 0x2D: return 'X';
        case 0x2E: return 'C';
        case 0x2F: return 'V';
        case 0x30: return 'B';
        case 0x31: return 'N';
        case 0x32: return 'M';
        case 0x33: return '<';
        case 0x34: return '>';
        case 0x35: return '?';
        case 0x39: return ' ';
        default: return '\0';

    }
    return '\0';
}

/* char findCapsAndShiftedChar(int c);
 * Inputs: c - The keyboard scancode that needs to be
               converted into a character.
 * Return Value: The character equivalent of the keyboard
                 scancode assuming caps lock is true and shift is pressed.
 * Function: Converts a keyboard scancode into either the
             lowercase letter or the secondary symbol on that key, 
             mimicing what happens when Caps lock is true and shift is pressed.
 */
char findCapsAndShiftedChar(int c){
    switch(c) {
        case 0x02: return '!';
        case 0x03: return '@';
        case 0x04: return '#';
        case 0x05: return '$';
        case 0x06: return '%';
        case 0x07: return '^';
        case 0x08: return '&';
        case 0x09: return '*';
        case 0x0A: return '(';
        case 0x0B: return ')';
        case 0x0C: return '_';
        case 0x0D: return '+';
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
        case 0x1A: return '{';
        case 0x1B: return '}';
        case 0x1E: return 'a';
        case 0x1F: return 's';
        case 0x20: return 'd';
        case 0x21: return 'f';
        case 0x22: return 'g';
        case 0x23: return 'h';
        case 0x24: return 'j';
        case 0x25: return 'k';
        case 0x26: return 'l';
        case 0x27: return ':';
        case 0x28: return '\"';
        case 0x29: return '~';
        case 0x2B: return '|';
        case 0x2C: return 'z';
        case 0x2D: return 'x';
        case 0x2E: return 'c';
        case 0x2F: return 'v';
        case 0x30: return 'b';
        case 0x31: return 'n';
        case 0x32: return 'm';
        case 0x33: return '<';
        case 0x34: return '>';
        case 0x35: return '?';
        case 0x39: return ' ';
        default: return '\0';   
    }
    return '\0';
}

/* char findCapsChar(int c);
 * Inputs: c - The keyboard scancode that needs to be
               converted into a character.
 * Return Value: The character equivalent of the keyboard
                 scancode assuming caps lock is true.
 * Function: Converts a keyboard scancode into either the
             uppercase letter or the primary symbol on that key, 
             mimicing what happens when Caps lock is true.
 */
char findCapsChar(int c){
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
        case 0x10: return 'Q';
        case 0x11: return 'W';
        case 0x12: return 'E';
        case 0x13: return 'R';
        case 0x14: return 'T';
        case 0x15: return 'Y';
        case 0x16: return 'U';
        case 0x17: return 'I';
        case 0x18: return 'O';
        case 0x19: return 'P';
        case 0x1A: return '[';
        case 0x1B: return ']';
        case 0x1E: return 'A';
        case 0x1F: return 'S';
        case 0x20: return 'D';
        case 0x21: return 'F';
        case 0x22: return 'G';
        case 0x23: return 'H';
        case 0x24: return 'J';
        case 0x25: return 'K';
        case 0x26: return 'L';
        case 0x27: return ';';
        case 0x28: return '\'';
        case 0x29: return '`';
        case 0x2B: return '\\';
        case 0x2C: return 'Z';
        case 0x2D: return 'X';
        case 0x2E: return 'C';
        case 0x2F: return 'V';
        case 0x30: return 'B';
        case 0x31: return 'N';
        case 0x32: return 'M';
        case 0x33: return ',';
        case 0x34: return '.';
        case 0x35: return '/';
        case 0x39: return ' ';
        default: return '\0';

    }
    return '\0';
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
    int SetFlag = 0;
    SetFlag = check_flags(Scancode);
    // printf("%d", textOverflow);
    if(ctrl == 1 && Scancode == 0x26) // 0x26 is L 
    {
        clear();
        reset_scrn_xy();
        memset(keyboard_buffer, '\0', sizeof(keyboard_buffer));
        send_eoi(0x01); // need to send eoi to irq_1
        return;

    }
    if (Scancode == 0x0F){  //scancode for tab button. put 4 spaces
        int i;
        for (i = 0; i < 3; i++){
            putc(' ');
        }
    }

    if(Scancode == 0x1C) // 1C is the "Enter " scancode
    {
        if(keyboard_buffer_index >= 128)
        {
            keyboard_buffer[127] = '\n'; // 127 is the last index of buffer 
            keyboard_buffer_index = 127;
        }
        else
        {
             keyboard_buffer[keyboard_buffer_index] = '\n';
             
        }
        // keyboard_buffer_index++;
        //terminal_test();
        putc('\n');
        //Prints the Keyboard buffer on enter pressed
        // terminal_read(0, terminal_buf, keyboard_buffer_index);
        // terminal_write(0, terminal_buf, keyboard_buffer_index);
        keyboard_buffer_index = 0;
        textOverflow = 0;
        gotNewLine = 1;

    }
    else if(Scancode == 0x0E) //0x0E is the "backspace" scancode
    {
        //Handle Backspace in buffer
        if(keyboard_buffer_index > 0)
        {
            keyboard_buffer_index--;
        }

        //Handle Backspace on screen
        if (getScreenX() == 0 && textOverflow > 0){
            
            move_line_up();    
            textOverflow -= 2; //-2 because putc(' ') adds one to textOverflow? Idk why or how
            putc(' ');
            move_line_up();

            //Make sure textOverflow is non-negative
            if(textOverflow < 0){ textOverflow = 0; }
        }
        else{
            decr_scrn_x();
            putc(' ');
            decr_scrn_x();
        }
        
        

    }   
    else
    {
        
        if(keyboard_buffer_index < 128 && Scancode < 0x81) 
        {
            char key;
            if(caps_lock == 1 && shift == 1){
                key = findCapsAndShiftedChar(Scancode);
            }

            else if(shift == 1)
            {
                key = findShiftedChar(Scancode);
            }
            else if(caps_lock == 1)
            {
                key = findCapsChar(Scancode);
            }
            else{

                key = findChar(Scancode);
            }
            if(key == '\0'){

            }
            //Print all keys that don't affect flags
            else if(SetFlag != 1){
                keyboard_buffer[keyboard_buffer_index] = key;
                keyboard_buffer_index++;
                putc(key);
            }
        }
        
    }
    send_eoi(0x01); // need to send eoi to irq_1
    
}

/* int check_flags(int Scancode);
 * Inputs: Scancode - The keyboard scancode that was received.
 * Return Value: 1 if a flag was modified, 0 if no flags were effected.
 * Function: Sets flags (shift, caps_lock, ctrl, etc.) based on the 
 *           Scancode that was received and returns whether any flags
 *           were changed.
 */
int check_flags(int Scancode)
{
 switch(Scancode) {
        case 0x3A: caps_lock = !caps_lock; 
        return 1;
        case 0x2A: shift = 1;
        return 1; // left shift
        case 0x36: shift = 1; 
        return 1;// right shift
        case 0xE0: alt = 1;
        return 1; // right alt
        case 0x38: alt = 1;
        return 1; // left alt
        case 0x1D: ctrl = 1; 
        return 1;
        case 0xAA: shift = 0; 
        return 1;// left shift release
        case 0xB6: shift = 0; 
        return 1;// right shift release
        case 0xB8: alt = 0;
        return 1;
        case 0x9D: ctrl = 0;
        return 1; // control release
        default:
            return 0;
 }
}
