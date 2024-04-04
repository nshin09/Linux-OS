#include "keyboard.h"
#include "lib.h"
#include "terminal.h"


int32_t terminal_open(const uint8_t* filename){
    return 0;
}

int32_t terminal_close(int32_t fd){
    return 0;
}

int32_t terminal_read(int32_t fd, void* buf, int32_t nbytes){
    int i = 0;
    // printf("in terminal read");
    // printf("\n");
    while (gotNewLine == 0){

    }
    gotNewLine = 0;
    while(i < keyboard_buffer_index && keyboard_buffer[i] != '\n' && i < 128 && i < nbytes ) 
    {
        ((char*)buf)[i] = keyboard_buffer[i];
        
        i++;
    }
    // keyboard_buffer_index = 0;
    return nbytes;
}


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


