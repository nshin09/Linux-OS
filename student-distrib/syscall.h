#ifndef SYSCALL
#define SYSCALL

#include "types.h"

/*
typedef struct __attribute__((packed)) page_table_t {
    uint32_t present        : 1;
    uint32_t read_write     : 1;
    uint32_t user_supervisor: 1;
    uint32_t write_through  : 1;
    uint32_t cache_disabled : 1;
    uint32_t accessed       : 1;
    uint32_t reserved       : 1;
    uint32_t page_size      : 1;
    uint32_t global_page    : 1;
    uint8_t  available      : 3;
    uint32_t addr           : 20;
} page_table_t;
*/

extern void syscall_handler_c(int call_num, int arg1, int arg2, int arg3);

//System calls needed for cp3
int32_t halt(uint8_t status);
extern int32_t execute(const uint8_t* command);
int32_t read(int32_t fd, void* buf, int32_t nbytes);
int32_t write(int32_t fd, const void* buf, int32_t nbytes);
int32_t open(const uint8_t* filename);
int32_t close(int32_t fd);
int32_t getargs(uint8_t* buf, int32_t nbytes);
int32_t vidmap(uint8_t** screen_start);
int32_t set_handler(int32_t signum, void* handler_address);
int32_t sigreturn(void);

#endif
