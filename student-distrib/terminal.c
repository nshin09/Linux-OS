#include "keyboard.h"
#include "lib.h"
#include "terminal.h"
#include "rtc.h"
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
    while(i < KBI_for_read && keyboard_buffer[i] != '\n' && i < 128 && i < nbytes ) 
    {
        ((char*)buf)[i] = keyboard_buffer[i];
        
        i++;
    }
    //Print buffer to check if this is working
    // printf("B->");
    // for(i=0; ((char*)buf)[i] != '\0' && ((char*)buf)[i] != '\n'; i++){
    //     putc(((char*)buf)[i]);
    // }
    // printf("<-\n");
    // printf("K->");
    // for(i=0; keyboard_buffer[i] != '\0' && keyboard_buffer[i] != '\n'; i++){
    //     putc(keyboard_buffer[i]);
    // }
    // printf("<-\n");

    // printf("Possible limiters are Keyboard_buffer_index: %d, nbytes: %d \n", KBI_for_read, nbytes);

    keyboard_buffer_index = 0;
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


