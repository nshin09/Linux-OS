#include "syscall.h"
#include "lib.h"


int32_t halt (uint8_t status){
    return 0;
}

int32_t execute (const uint8_t* command){
    //First, Check if given filename is an executable filetype

    return 0;
}

int32_t read (int32_t fd, void* buf, int32_t nbytes){
    //Get File from File Descriptor Table?
    //file = fdt[fd]

    return 0;
}

int32_t write (int32_t fd, const void* buf, int32_t nbytes){
    //Get File from File Descriptor Table?
    //file = fdt[fd]

    return 0;
}

int32_t open (const uint8_t* filename){
    return 0;
}

int32_t close (int32_t fd){
    //Get File from File Descriptor Table?
    //file = fdt[fd]

    return 0;
}

int32_t getargs (uint8_t* buf, int32_t nbytes){
    return 0;
}

int32_t vidmap (uint8_t** screen_start){
    return 0;
}

//These two system calls are extra credit according to the footer on the assignment
int32_t set_handler (int32_t signum, void* handler_address){
    return 0;
}
int32_t sigreturn (void){
    return 0;
}


void syscall_handler_c(int call_num, int arg1, int arg2, int arg3){
    printf("Got system call %d, gets args %d, %d, and %d\n", call_num, arg1, arg2, arg3);

    //JUST FOR TESTING SET CALL_NUM
    //call_num = 4;

    int syscall_out;
    switch(call_num) {
        case 1: 
            syscall_out = halt((uint8_t)arg1);
            break;
        case 2: 
            syscall_out = execute((const uint8_t*)arg1);
            break;
        case 3: 
            syscall_out = read((uint32_t)arg1, (void*)arg2, (int32_t)arg3);
            break;
        case 4: 
            syscall_out = write((uint32_t)arg1, (const void*)arg2, (int32_t)arg3);
            break;
        case 5:
            syscall_out = open((const uint8_t*)arg1);
            break;
        case 6:
            syscall_out = close((uint32_t)arg1);
            break;
        case 7:
            syscall_out = getargs((uint8_t*)arg1, (uint32_t)arg2);
            break;
        case 8:
            syscall_out = vidmap((uint8_t**)arg1);
            break;
        case 9:
            syscall_out = set_handler((int32_t)arg1, (void*)arg2);
            break;
        case 10:
            syscall_out = sigreturn();
            break;
    }

    printf("Sys call %d got %d\n", call_num, syscall_out);
    return;
}
