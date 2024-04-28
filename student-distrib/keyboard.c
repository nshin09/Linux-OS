#include "lib.h"
#include "i8259.h"
#include "keyboard.h"
#include "terminal.h"
#include "x86_desc.h"
#include "syscall_asm.h"
#include "paging.h"
#include "syscall.h"

/* void SetupTerminals();
 * Inputs: Terminal_instance_t empty
 * Return Value: None
 * Function: Sets global array of 3 terminals to an empty placeholder terminal. 
   Zeroes out vars used for scheduling.
 */

void SetupTerminals(Terminal_instance_t empty){
    Terminals[0] = empty;
    Terminals[1] = empty;
    Terminals[2] = empty;

    StartTerminal(0);
    ActiveTerminal = 0;
    VisibleTerminal = 0;
    ScheduledTerminal = 0;
}

void StartTerminal(int idx){
    Terminals[idx].Started = 1;
    Terminals[idx].Active = 1;
}

/* void initialize_keyboard();
 * Inputs: None
 * Return Value: None
 * Function: Enables IRQ1 so that the keyboard
 *           interrupts can be received.
 */
void initialize_keyboard(){
    enable_irq(1);

    keyboard_buffer_index[0] = 0;
    keyboard_buffer_index[1] = 0;
    keyboard_buffer_index[2] = 0;

    caps_lock = 0;
    shift = 0;
    ctrl = 0;
    alt = 0;
    textOverflow = 0;

    gotNewLine[0] = 0;
    gotNewLine[1] = 0;
    gotNewLine[2] = 0;

    ActiveTerminal = 0;
    VisibleTerminal = 0;
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
    //set Shift, capslock, etc.
    int SetFlag = 0;
    SetFlag = check_flags(Scancode);
    //Switch Terminal
    if(alt == 1){
        if((Scancode == 0x3b && VisibleTerminal != 0) || (Scancode == 0x3c && VisibleTerminal != 1)|| (Scancode == 0x3d && VisibleTerminal != 2)){
            
            int SwitchTo = 0;
            if(Scancode == 0x3b){ 
                SwitchTo = 0; }
            else if(Scancode == 0x3c){ 
                SwitchTo = 1; }
            else if(Scancode == 0x3d){
                SwitchTo = 2; }

            //VisibleTerminal = SwitchTo;

            SwitchVisible(SwitchTo);

            return;
        }
    }
    // printf("%d", textOverflow);
    if(ctrl == 1 && Scancode == 0x26) // 0x26 is L 
    {
        clear();
        reset_scrn_xy();
        memset(keyboard_buffer[VisibleTerminal], '\0', sizeof(keyboard_buffer[VisibleTerminal]));
        send_eoi(0x01); // need to send eoi to irq_1
        return;

    }
    if (Scancode == 0x0F){  //scancode for tab button. put 4 spaces
        int i;
        for (i = 0; i < 3; i++){
            putc_screen(' ');
        }
    }

    if(Scancode == 0x1C) // 1C is the "Enter " scancode
    {
        if(keyboard_buffer_index[VisibleTerminal] >= 128)
        {
            keyboard_buffer[VisibleTerminal][127] = '\n'; // 127 is the last index of buffer 
            keyboard_buffer_index[VisibleTerminal] = 127;
        }
        else
        {
             keyboard_buffer[VisibleTerminal][keyboard_buffer_index[VisibleTerminal]] = '\n';
             
        }
        // keyboard_buffer_index++;
        //terminal_test();
        putc_screen('\n');
        //Prints the Keyboard buffer on enter pressed
        // terminal_read(0, terminal_buf, keyboard_buffer_index);
        // terminal_write(0, terminal_buf, keyboard_buffer_index);

        //Set read function's buffer index and reset buffer index
        KBI_for_read = keyboard_buffer_index[VisibleTerminal];
        keyboard_buffer_index[VisibleTerminal] = 0;

        textOverflow = 0;
        gotNewLine[VisibleTerminal] = 1;
        // if(VisibleTerminal != ActiveTerminal){
        //     putc_screen(' ');
        //     putc_screen('!');
        //     putc_screen('!');
        //     putc_screen('!');
        //     putc_screen(' ');
        // }
        // printf("GNL %d:%d:%d \n", gotNewLine[0], gotNewLine[1], gotNewLine[2]);

    }
    else if(Scancode == 0x0E && keyboard_buffer_index[VisibleTerminal] > 0) //0x0E is the "backspace" scancode
    {
        //Handle Backspace in buffer
        if(keyboard_buffer_index[VisibleTerminal] > 0)
        {
            keyboard_buffer_index[VisibleTerminal]--;
        }

        //Handle Backspace on screen
        if (getScreenX() == 0 && textOverflow > 0){
            
            move_line_up();    
            textOverflow -= 2; //-2 because putc(' ') adds one to textOverflow? Idk why or how
            putc_screen(' ');
            move_line_up();

            //Make sure textOverflow is non-negative
            if(textOverflow < 0){ textOverflow = 0; }
        }
        else{
            decr_scrn_x();
            putc_screen(' ');
            decr_scrn_x();
        }
        
        

    }   
    else
    {
        
        if(keyboard_buffer_index[VisibleTerminal] < 128 && Scancode < 0x81) 
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
                keyboard_buffer[VisibleTerminal][keyboard_buffer_index[VisibleTerminal]] = key;
                keyboard_buffer_index[VisibleTerminal]++;
                putc_screen(key);
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

/* void SwitchTerminal(int SwitchTo);
 * Inputs: SwitchTo - The terminal to switch to and start executing.
 * Return Value: None
 * Function: Switches the function that is currently being run to the
 *           program currently running on the given terminal.
 */
void SwitchTerminal(int SwitchTo){
    // //Save active video memory to the active terminal's memory
    // void* ActiveMem = (void*)(0xB9000 + ActiveTerminal*0x1000);
    // memcpy(ActiveMem, (void*)0xB8000, 0x1000);
    
    // //Save current Active terminals EBP, ESP, etc.
    // Terminals[ActiveTerminal].cursor_x = getScreenX();
    // Terminals[ActiveTerminal].cursor_y = getScreenY();
    Terminals[ActiveTerminal].EBP = Get_EBP();
    Terminals[ActiveTerminal].ESP = Get_ESP();
    Terminals[ActiveTerminal].PID = Get_PID();

    //Change ActiveTerminal
    ActiveTerminal = SwitchTo;

    // //Copy the new terminal's memory into active memory
    // memcpy((void*)0xB8000, (void*)(0xB9000 + ActiveTerminal*0x1000), 0x1000);

    // //set new Active terminals EBP, ESP, etc.

    // //reset cursor
    // SetXY(Terminals[ActiveTerminal].cursor_x, Terminals[ActiveTerminal].cursor_y);
    // printf("Terminal 0 has x %d, y %d\n", Terminals[0].cursor_x, Terminals[0].cursor_y);
    // printf("Terminal 1 has x %d, y %d\n", Terminals[1].cursor_x, Terminals[1].cursor_y);
    // printf("Terminal 2 has x %d, y %d\n", Terminals[2].cursor_x, Terminals[2].cursor_y);
    //reset_scrn_xy();

    Set_PID(Terminals[ActiveTerminal].PID);

    //Change Paging

    //printf("Switching pre-EOI");
    send_eoi(0x01);
    send_eoi(0x0);

    // If it's the first time in this terminal, start the terminal
    // and call execute
    if(Terminals[ActiveTerminal].Started == 0){
        StartTerminal(ActiveTerminal);
        Open_PID(ActiveTerminal);
        execute_local((uint8_t*)"shell", 1);
    }
    int pd_idx = 32; //The page directory index of virtual address 128MB
    page_directory[pd_idx].addr = (0x800000 + (Terminals[ActiveTerminal].PID)*0x400000)>> 12;
    flush_tlb();

    tss.esp0 = 0x800000 - (8192*Terminals[ActiveTerminal].PID);
    Set_EBP_ESP(Terminals[ActiveTerminal].EBP, Terminals[ActiveTerminal].ESP);
}

/* void SwitchVisible(int SwitchTo);
 * Inputs: SwitchTo - The terminal to make visible.
 * Return Value: None
 * Function: Switches the terminal that is currently being displayed
 *           to the provided terminal. Also allows this terminal to get
 *           keyboard inputs.
 */
void SwitchVisible(int SwitchTo){
    //Save active video memory to the active terminal's memory
    void* ActiveMem = (void*)(0xB9000 + VisibleTerminal*0x1000);
    memcpy(ActiveMem, (void*)0xB8000, 0x1000);
    
    //Save current Active terminals EBP, ESP, etc.
    Terminals[VisibleTerminal].cursor_x = getScreenX();
    Terminals[VisibleTerminal].cursor_y = getScreenY();

    VisibleTerminal = SwitchTo;

    //Copy the new terminal's memory into active memory
    memcpy((void*)0xB8000, (void*)(0xB9000 + VisibleTerminal*0x1000), 0x1000);

    //reset cursor
    SetXY(Terminals[VisibleTerminal].cursor_x, Terminals[VisibleTerminal].cursor_y);

    if(Terminals[VisibleTerminal].Started == 0){
        SwitchTerminal(VisibleTerminal);
    }
}
