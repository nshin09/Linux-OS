#ifndef SYSCALL
#define SYSCALL

#include "types.h"
#include "file_system.h"

#define MAX_FD_ENTRIES 8 //8 totalk files alowed

typedef struct  PCB_t {
    uint32_t Active : 1;
    uint32_t PID;
    uint32_t Parent_PID;
    uint32_t EBP;
    fdt_entry_t FDT[MAX_FD_ENTRIES];
    uint8_t syscall_args[32];
} PCB_t;

// extern void syscall_handler_c(int call_num, int arg1, int arg2, int arg3);
extern void initialize_fop();
#define max_PCB  6
#define PCB_size  8192     //8 kilabytes
// #define CURR_MEM  0x800000 //8 Megabyes
// #define MAX_FILE_VALUE  40000
PCB_t* Get_PCB_ptr (int local_PID);
// Null functions
int32_t null_read(int32_t fd, void* buf, int32_t nbytes);
int32_t null_write(int32_t fd, const void* buf, int32_t nbytes);
int32_t null_open(const uint8_t* filename);
int32_t null_close(int32_t fd);

// fop tables
fop_table_t null_fop;
fop_table_t stdin_fop;
fop_table_t stdout_fop;
fop_table_t rtc_fop;
fop_table_t directory_fop;
fop_table_t file_fop;


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
