#include "file_system.h"
#include "rtc.h"
#include "lib.h"
#include "syscall.h"

/* void file_system_init(module_t* start)
 * Inputs: start- the starting address specified in kernel
 * Return Value: void
 * Function: ninitializes the core pointers for file system*/
void file_system_init(module_t* start)
{
    boot_block_ptr = (boot_block_t*)start;
    inodes_ptr = (inodes_t*)(boot_block_ptr + 1);
    entry_ptr = (dentry_t*)(boot_block_ptr->dir_entries);
    data_block_ptr = (uint8_t*)(inodes_ptr + (boot_block_ptr->num_inodes));
    int i;
    for(i=0; i<FD_TABLE_ENTRIES;i++){
        fd_table[i].fop_table_ptr = NULL;
        fd_table[i].flags = 0;
        fd_table[i].file_position = 0;
        fd_table[i].inode = 0;

        if(i==0 || i==1){
            fd_table[i].flags = 1; //0 and 1 idx occupied for stdin and stdout
        }
    }

}

/* int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length)
 * Inputs: inode - the current inode we want to read from
            buf - buffer we will be populating with inode data
            offset - offset into inode
            length - number of bytes to be copied
 * Return Value: number of bytes copied, -1 on fail
 * Function: Reads a specific files data and stores it into a buffer*/
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length)
{
    int i = 0;
    uint32_t* block_ptr;
    inodes_t* curr_inode = (inodes_t*)(inodes_ptr + inode);
    uint32_t file_length = curr_inode->length;
    // printf("Read data len %d, file_len %d ", length, file_length);
    // check for bounds.
    if(offset >  file_length || inode > boot_block_ptr->num_inodes)
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
        block_ptr = (uint32_t*)(data_block_ptr + byte_index + (4096 * block_number)); // find where you want to write to buf
        // printf("%c", (uint8_t)(*block_ptr));
        *buf = (uint8_t)(*block_ptr); // not quite sure if this works the way i want it to?
        // memcpy(buf,block_ptr, 1);
        i++;
        byte_index++;
        buf++; // increment values for next iteration
    }

    //printf(":%d\n",i);
    return i;
}

/* int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry)
 * Inputs: fname - the file name we are searching for
            dentry - current directory we enter
 * Return Value: 0 if we found the file name, -1 if we did not
 * Function: Looks for the specified file name within a dentry*/
int32_t read_dentry_by_name(const uint8_t* fname, dentry_t* dentry) //dentry->node_num
{
    // printf("\nstrlen after: %d", strlen(fname));
  
    // int length = strlen((int8_t*)fname);
    int length = 0;
    while (length<32 && fname[length] != '\0'){
        length++;
    }

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
         match = strncmp((int8_t*)fname,(int8_t*)entry_ptr[i].file_name,length); // max file name is 32 bytes
        if(length == entry_length && match == 0)
        {
            strncpy((int8_t*)dentry->file_name,(int8_t*)entry_ptr[i].file_name,length); // copy name, node number, and file type to the dentry parameter. 
            dentry->node_num = entry_ptr[i].node_num;
            dentry->file_type = entry_ptr[i].file_type;
            return 0;
        }
    }

    return -1; //if loop terminates that means we could not find matching name. 
}

/* int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry)
 * Inputs: index - finds the dentry at the specified index 
            dentry - current directory we enter
 * Return Value: 0 if file exists, -1 if not
 * Function: Looks for the specified index within a dentry*/
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

/* int32_t file_read(int32_t fd, void* buf, int32_t nbytes)
 * Inputs: int32_t fd - file descriptor to index into file descriptor table, specifies file 
           void* buf buffer to read into
           int32_t nbytes - num of bytes to read
 * Return Value: 0 if file exists, -1 if not
 * Function: reads nbytes of data from file*/
int32_t file_read(int32_t fd, void* buf, int32_t nbytes)
{
    //printf(" file_read fd:%d, nbytes:%d\n", fd, nbytes);
    if (fd<0 || fd>MAX_FD_ENTRIES){
        return -1;
    }
    PCB_t* PCB = Get_PCB_ptr(-1); // -1 means we just want to get the current PID value. 
    int ret = read_data(PCB->FDT[fd].inode, PCB->FDT[fd].file_position, buf, nbytes);
    if(ret == -1)
    {
        return -1;
    }
    PCB->FDT[fd].file_position = PCB->FDT[fd].file_position+ret;   //edit file position to read from so we dont read again
    return ret;
}
/* int32_t directory_read(int32_t fd, void* buf, int32_t nbytes)
 * Inputs: int32_t fd - file descriptor to index into file descriptor table, specifies file 
           void* buf buffer to read into
           int32_t nbytes - num of bytes to read
 * Return Value: 0 if file exists, -1 if not
 * Function: Looks for the specified index within a directory*/
int32_t directory_read(int32_t fd, void* buf, int32_t nbytes)
{
    int i;
    if (fd<0 || fd>MAX_FD_ENTRIES){
        return -1;
    }
    PCB_t* PCB = Get_PCB_ptr(-1); // -1 means we just want to get the current PID value. 
    dentry_t temp_dentry;
    int read_by_idx = read_dentry_by_index(PCB->FDT[fd].file_position, &temp_dentry);
    if (read_by_idx == -1){
        return -1;
    }
    int length = strlen((int8_t*)temp_dentry.file_name);
    for(i = 0; i < 32 && i < length; i++)
    {
        ((uint8_t*)buf)[i] =  temp_dentry.file_name[i];
    }
    // strncpy((int8_t*)buf, (int8_t*)temp_dentry.file_name, sizeof(temp_dentry.file_name));
    PCB->FDT[fd].file_position++;
    if (length>32){
        length = 32;
    }
    return length;
}

/* int32_t file_open(const uint8_t* filename)
 * Inputs: filename - filename we are looking to open
 * Return Value: 0 if file exists, -1 if not
 * Function: Opens a specified file*/
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

