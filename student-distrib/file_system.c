#include "file_system.h"


void file_system_init(module_t* start)
{
    boot_block_ptr = (boot_block_t*)start;
    inodes_ptr = (inodes_t*)(boot_block_ptr + 1);
    entry_ptr = (dir_entry_t*)(boot_block_ptr->dir_entries);
    data_block_ptr = (uint8_t*)(inodes_ptr + (boot_block_ptr->num_inodes));
}
int32_t read_data (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length)
{
    int i = 0;
    uint32_t* block_ptr;
    inodes_t* curr_inode = (inodes_t*)(inodes_ptr + inode);
    uint32_t file_length = curr_inode->length;
    // check for bounds. 
    if(offset >=  file_length || inode > boot_block_ptr->num_inodes)
    {
        return -1;
    }
    // 4096 is equivalent to 4KB, which is the size of a single block. 
    uint32_t block_index = offset / 4096; //indicates which block you are on
    uint32_t byte_index = offset % 4096; // indicates how far within the block you are 
    uint32_t block_number;
    while(i < length && (i+offset) < file_length) 
    {
        if(byte_index >= 4096) // if the end of the block has been reached
        {
            byte_index = 0; // reset 
            block_index++; // increment the block we are looking at
        }
        block_number = curr_inode->block_num[block_index];
        if(block_number >= boot_block_ptr->num_blocks) // make sure block number is within valid range.
        {
            return -1;
        }
        block_ptr = (uint32_t*)(data_block_ptr + byte_index + (4096 * block_index)); // find where you want to write to buf
        *buf = (uint8_t)(*block_ptr); // not quite sure if this works the way i want it to?

        i++;
        byte_index++;
        buf++; // increment values for next iteration
    }

    return i;
}
int32_t file_open(int32_t fd)
{
    return 0;
}
int32_t directory_open(int32_t fd)
{
    return 0;
}
int32_t file_close(int32_t fd)
{
    return 0;
}

int32_t directory_close(int32_t fd)
{
    return 0;
}

// file_write does nothing.
int32_t file_write()
{
    return -1;
}

// directory_write does nothing. 
int32_t directory_write()
{
    return -1;
}

