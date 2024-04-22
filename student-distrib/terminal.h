#ifndef TERMINAL_H_
#define TERMINAL_H_

// typedef struct  Terminal_t {
//     uint32_t Started : 1; //1 if the terminal has been started (even if it has since been quit)
//     uint32_t Active : 1;
//     uint32_t Vidmem_location;
//     uint32_t EBP;
//     uint32_t ESP;
//     uint32_t EIP;
//     int cursor_x;
//     int cursor_y;
// } Terminal_t;

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
