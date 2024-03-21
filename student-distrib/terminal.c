#include "keyboard.h"
#include "lib.h"
#include "terminal.h"


int32_t terminal_open(const uint8_t* filename){
    return 0;
}

int32_t terminal_close(int32_t fd){
    return 0;
}

int32_t terminal_read(int32_t fd, char* buf, int32_t nbytes){
    int i = 0;
    while(i < keyboard_buffer_index && keyboard_buffer[i] != '\n' && i < nbytes ) 
    {
        ((char*)buf)[i] = keyboard_buffer[i];
        
        // printf("%c -> %c \n", keyboard_buffer[i], buf[i]);

        i++;
    }
    // keyboard_buffer_index = 0;
    return nbytes;
}

int32_t terminal_write(int32_t fd, const char* buf, int32_t nbytes)
{
    int i;


    for(i =0; i < nbytes; i++){
        putc(((char*)buf)[i]); 
        // printf("%c", ((char*)buf)[i]);

    }

    //  while(i < nbytes && ((char*)buf)[i] != '\n')
    //  {
    //     putc(((char*)buf)[i]);
    //     i++;
    //  }
     return nbytes;
}

