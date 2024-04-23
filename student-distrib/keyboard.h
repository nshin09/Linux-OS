#include "types.h"
#include "x86_desc.h"
#ifndef KEYBOARD_H
#define KEYBOARD_H

//initializes the terminal structures
extern void SetupTerminals(Terminal_instance_t);
extern void StartTerminal(int idx);
//Initializes the keyboard
extern void initialize_keyboard();

//Reads a scancode from the keyboard and prints it on screen
extern void keyboard_handler();

//checks for if flags should be set
extern int check_flags(int Scancode);
//flags
int caps_lock;
int shift;
int ctrl;
int alt;
int gotNewLine;


//Converts a scancode to a character
char findChar(int c);
char findShiftedChar(int c);
char findCapsChar(int c);

//Buffers to hold what has been typed on the keyboard
char keyboard_buffer[3][128]; 
char terminal_buf[128];
Terminal_instance_t Terminals[3];
//Index of the first unfilled space in the keyboard_buffer
int32_t keyboard_buffer_index;
int32_t KBI_for_read;

#endif
