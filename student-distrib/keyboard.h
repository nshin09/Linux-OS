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
//Tracks new line for each of the 3 terminals
int gotNewLine[3];


//Converts a scancode to a character
char findChar(int c);
char findShiftedChar(int c);
char findCapsChar(int c);

//Switches the active terminal to SwitchTo
void SwitchTerminal(int SwitchTo);
//Switches the visible terminal to SwitchTo
void SwitchVisible(int SwitchTo);

//Buffers to hold what has been typed on the keyboard
char keyboard_buffer[3][128]; 
char terminal_buf[128];
//Index of the first unfilled space in the keyboard_buffer
Terminal_instance_t Terminals[3];
int32_t keyboard_buffer_index[3];
int32_t KBI_for_read;

#endif
