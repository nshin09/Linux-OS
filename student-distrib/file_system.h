#ifndef FILE_SYSTEM_H_
#define FILE_SYSTEM_H_
#include "types.h"
#include "lib.h"
#include "multiboot.h"
// initialize the starts to dir entry, boot_block, and inodes. 
void file_system_init(module_t* start);
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);


int32_t file_open(int32_t fd);
int32_t file_close(int32_t fd);
int32_t file_write();
void file_read();

int32_t directory_open(int32_t fd);
int32_t directory_close(int32_t fd);
int32_t directory_write();
void directory_read();
typedef struct dir_entry {
   uint8_t file_name[32];
   uint32_t file_type;
   uint32_t node_num;
   uint8_t reserved[24];
} dir_entry_t;

typedef struct boot_block {
    uint32_t num_d_entries;
    uint32_t num_inodes;
    uint32_t num_blocks;
    uint8_t reserved[52]; // reserved number
    dir_entry_t dir_entries[63]; // number of directory entries, (64*63) + 52 + 4 + 4 + 4 = 4096 bytes. 
} boot_block_t;

typedef struct inodes {
    uint32_t length;
    uint32_t block_num[1023]; // 1023 is the max number of data blocks
} inodes_t;

boot_block_t* boot_block_ptr;
inodes_t* inodes_ptr;
dir_entry_t* entry_ptr;
uint8_t* data_block_ptr;

#endif
