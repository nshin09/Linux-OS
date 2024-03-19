#ifndef _PAGING_H_
#define _PAGING_H_
#include "types.h"
#define VGA_MEM_START 0xB8000
#define KERNEL_MEMORY 0x400000

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


// to differentiate, page size, or the 26th bit, is 1 when 4mb page, 0 else
typedef union page {
    // 4kb page
    struct {
        uint32_t present            : 1;
        uint32_t readwrite          : 1;
        uint32_t user_supervisor    : 1;
        uint32_t write_through      : 1;
        uint32_t cache_disabled     : 1;
        uint32_t accessed           : 1;
        uint32_t dirty              : 1;
        uint32_t PAT                : 1;
        uint32_t global_page        : 1;
        uint8_t  available          : 3;
        uint32_t addr               : 20;
    } __attribute__ ((packed));

    // 4mb page
    struct {
        uint32_t present            : 1;
        uint32_t read_write         : 1;
        uint32_t user_supervisor    : 1;
        uint32_t write_through      : 1;
        uint32_t cache_disabled     : 1;
        uint32_t accessed           : 1;
        uint32_t dirty              : 1;
        uint32_t page_size          : 1;
        uint32_t global_page        : 1;
        uint8_t  available          : 3;
        uint32_t PAT                : 1;
        uint32_t reserved           : 9;
        uint32_t addr               : 10;
    } __attribute__ ((packed));

} page_t;

//The page directory. Set up in paging.c
extern page_table_t page_directory[1024] __attribute__((aligned(4096)));
extern page_t page_table[1024] __attribute__((aligned(4096)));
// extern page_t page_VGA[1024] __attribute__((aligned(4096))); unused array


extern void initPaging();

#endif
