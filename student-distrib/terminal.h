#ifndef TERMINAL_H_
#define TERMINAL_H_

// int rtc_mode;
#include "types.h"
// opens the terminal
int32_t terminal_open(const uint8_t* filename);
// closes the terminal
int32_t terminal_close(int32_t fd);

/* read from keyboard buffer and put it into the buf specified  */
int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes);
/* write the specified buffer to video memory  */
int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes);

int ActiveTerminal;

#endif 
