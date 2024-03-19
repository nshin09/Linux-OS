#include "types.h"
#ifndef _PAGING_H_
#define _PAGING_H_

typedef struct __attribute__ ((packed)) page_table {
        uint32_t addr               : 20;
        uint8_t available           : 3;
        uint32_t global_page        : 1;
        uint32_t page_size          : 1;
        uint32_t reserved           : 1;
        uint32_t accessed           : 1;
        uint32_t cache_disabled     : 1;
        uint32_t write_through      : 1;
        uint32_t user_supervisor    : 1;
        uint32_t read_write         : 1;
        uint32_t present            : 1;
            
} page_table_t;

typedef union page {
    //4kb page
        struct {
            uint32_t addr               : 20;
            uint8_t  available          : 3;
            uint32_t global_page        : 1;
            uint32_t PAT                : 1;
            uint32_t dirty              : 1;
            uint32_t accessed           : 1;
            uint32_t cache_disabled     : 1;
            uint32_t write_through      : 1;
            uint32_t user_supervisor    : 1;
            uint32_t read_write         : 1;
            uint32_t present            : 1;
        } __attribute__ ((packed));

    //4mb page
        struct {
            uint32_t addr               : 10;
            uint32_t reserved           : 9;
            uint32_t PAT                : 1;
            uint8_t  available          : 3;
            uint32_t global_page        : 1;
            uint32_t page_size          : 1;
            uint32_t dirty              : 1;
            uint32_t accessed           : 1;
            uint32_t cache_disabled     : 1;
            uint32_t write_through      : 1;
            uint32_t user_supervisor    : 1;
            uint32_t read_write         : 1;
            uint32_t present            : 1;
        } __attribute__ ((packed));

} page_t;

extern uint32_t directory[1024] __attribute__((aligned(4096)));

extern void initPaging();

#endif
