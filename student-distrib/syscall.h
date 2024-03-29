#ifndef SYSCALL
#define SYSCALL

#include "types.h"
#include "file_system.h"


typedef struct  PCB_t {
    uint32_t Active : 1;
    uint32_t PID;
    uint32_t Parent_PID;
    uint32_t EBP;
    fdt_entry_t FDT[8];
    uint32_t syscall_args[4];
} PCB_t;

extern void syscall_handler_c(int call_num, int arg1, int arg2, int arg3);

#define max_PCB  6
#define PCB_size  8192     //8 kilabytes
#define curr_mem  0x800000 //8 Megabyes

//System calls needed for cp3
int32_t halt(uint8_t status);
extern int32_t execute(const uint8_t* command);
int32_t read(int32_t fd, void* buf, int32_t nbytes);
int32_t write(int32_t fd, const void* buf, int32_t nbytes);
int32_t open(const uint8_t* filename);
int32_t close(int32_t fd);
extern int32_t getargs(uint8_t* buf, int32_t nbytes);
int32_t vidmap(uint8_t** screen_start);
int32_t set_handler(int32_t signum, void* handler_address);
int32_t sigreturn(void);

#endif
