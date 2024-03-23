#include "types.h"
#ifndef KEYBOARD_H
#define KEYBOARD_H

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


//Converts a scancode to a character
char findChar(int c);
char findShiftedChar(int c);
char findCapsChar(int c);
char keyboard_buffer[128]; 
char buf[128];
int32_t keyboard_buffer_index;

#endif
