#include "keyboard.h"
#include "lib.h"
#include "terminal.h"
#include "rtc.h"


/* void: terminal open and close
 * Inputs: filename/fd
 * Return Value: 0, not implemented
 * Function: N/A */

int32_t terminal_open(const uint8_t* filename){
    return 0;
}

int32_t terminal_close(int32_t fd){
    return 0;
}

/* int32: terminal_read(int32_t fd, void* buf, int32_t nbytes)
 * Inputs: file descriptor, buffer, num bytes
 * Return Value: nbytes read
 * Function: reads data from the keyboard, read from keyboard buffer and put it into the buf specified */

int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes){
    int i = 0;
    gotNewLine[VisibleTerminal] = 0;
    // printf(" -In terminal read %d trying %d", ActiveTerminal, VisibleTerminal);
    // printf("\n");
    while (gotNewLine[VisibleTerminal] == 0 || ActiveTerminal != VisibleTerminal){
        //printf("|%d:%d:%d| ", gotNewLine[0], gotNewLine[1], gotNewLine[2]);
    }
    gotNewLine[VisibleTerminal] = 0;

    while(i<128) 
    {
        ((char*)buf)[i] = '\0';
        i++;
    }
    i = 0;
    while((i < KBI_for_read) && (keyboard_buffer[VisibleTerminal][i] != '\n') && (i < 128) && (i < nbytes)) 
    {
        ((char*)buf)[i] = keyboard_buffer[VisibleTerminal][i];   
        keyboard_buffer[VisibleTerminal][i] = '\0';
        i++;
    }
    //Print buffer to check if this is working

    keyboard_buffer_index[VisibleTerminal] = 0;
    return nbytes;
}

/* int32: terminal_write(int32_t fd, void* buf, int32_t nbytes)
 * Inputs: file descriptor, buffer, num bytes
 * Return Value: nbytes written
 * Function: writes data to the terminal, all data displayed to the screen immediately.*/

int32_t terminal_write(int32_t fd, const void* buf, int32_t nbytes)
{
    int i = 0;

    // printf("t write");
    while(i < nbytes && ((char*)buf)[i]!= '\0')
    {
        putc(((char*)buf)[i]);
        i++;
    } 

    // printf("\n");
    return nbytes;
}


