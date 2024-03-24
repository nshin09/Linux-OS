#include "file_system.h"
#include "lib.h"

void file_system_init(module_t* start)
{
    boot_block_ptr = (boot_block_t*)start;
    inodes_ptr = (inodes_t*)(boot_block_ptr + 1);
    entry_ptr = (dentry_t*)(boot_block_ptr->dir_entries);
    data_block_ptr = (uint8_t*)(inodes_ptr + (boot_block_ptr->num_inodes));
}
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length)
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
        // memcpy(buf,block_ptr, 1);
        i++;
        byte_index++;
        buf++; // increment values for next iteration
    }

    return i;
}
int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry) //dentry->node_num
{
    int length = strlen((int8_t*)fname);
    if(length > 32)
    {
        length = 32;
    }
    int entry_length;
    int match = 0;
    int i;
    for(i = 0; i < 63; i++) // iterate through each block
    {
        entry_length = strlen((int8_t*)entry_ptr[i].file_name);
        if(entry_length > 32)
        {
            entry_length = 32;
        }
         match = strncmp((int8_t*)fname,(int8_t*)entry_ptr[i].file_name,32); // max file name is 32 bytes
        if(length == entry_length && match == 0)
        {
            strncpy((int8_t*)dentry->file_name,(int8_t*)entry_ptr[i].file_name,32); // copy name, node number, and file type to the dentry parameter. 
            dentry->node_num = entry_ptr[i].node_num;
            dentry->file_type = entry_ptr[i].file_type;
            return 0;
        }
    }

    return -1; //if loop terminates that means we could not find matching name. 
}
int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry)
{
    if(index >= 63) // 63 is the number of directory entries in the bootblock.
    {
        return -1;
    }
    strncpy((int8_t*)dentry->file_name,(int8_t*)entry_ptr[index].file_name,32); // copy name, node number, and file type to the dentry parameter. 
    dentry->node_num = entry_ptr[index].node_num;
    dentry->file_type = entry_ptr[index].file_type;
    return 0;
}

int32_t file_read(int32_t fd, void* buf, int32_t nbytes)
{
    
    return 0;
}

int32_t directory_read(int32_t fd, void* buf, int32_t nbytes)
{
    return 0;
}

int32_t file_open(const uint8_t* filename)
{

    return 0;
}
int32_t directory_open(const uint8_t* filename)
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
int32_t file_write(int32_t fd, const void* buf, int32_t nbytes)
{
    return -1;
}

// directory_write does nothing. 
int32_t directory_write(int32_t fd, const void* buf, int32_t nbytes)
{
    return -1;
}

