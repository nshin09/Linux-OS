#include "types.h"
#ifndef KEYBOARD_H
#define KEYBOARD_H

//Initializes the keyboard
extern void initialize_keyboard();

//Reads a scancode from the keyboard and prints it on screen
extern void keyboard_handler();

//Converts a scancode to a character
char findChar(int c);

 char keyboard_buffer[128]; 
 int32_t keyboard_buffer_index;

#endif
