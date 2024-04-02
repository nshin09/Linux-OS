#include "syscall.h"
#include "syscall_asm.h"
#include "lib.h"
#include "file_system.h"
#include "paging.h"
#include "rtc.h"
#include "terminal.h"
#include "x86_desc.h"
//Moved these variables to this file cause they weren't being seen properly
const int CURR_MEM = 0x800000; //8 Megabyes
const int MAX_FILE_VALUE = 40000;

int PID = 0;
int PID_ARRAY[6] = {0,0,0,0,0,0};
uint8_t args[128] = {'\0'};
void initialize_fop()
{
    null_fop.open = null_open;
    null_fop.close = null_close;
    null_fop.read = null_read;
    null_fop.write = null_write; 

    stdin_fop.open = terminal_open;
    stdin_fop.close = terminal_close;
    stdin_fop.read = terminal_read;
    stdin_fop.write = null_write;

    stdout_fop.open = terminal_open;
    stdout_fop.close = terminal_close;
    stdout_fop.read = null_read;
    stdout_fop.write = terminal_write;

    rtc_fop.open = rtc_open;
    rtc_fop.close = rtc_close;
    rtc_fop.read = rtc_read;
    rtc_fop.write = rtc_write;

    directory_fop.open = directory_open;
    directory_fop.close = directory_close;
    directory_fop.read = directory_read;
    directory_fop.write = directory_write;

    file_fop.open = file_open;
    file_fop.close = file_close;
    file_fop.read = file_read;
    file_fop.write = file_write;
}

PCB_t* Get_PCB_ptr (int local_PID){
    return (PCB_t*)(CURR_MEM - (local_PID+1)*PCB_size);
}

void Flush_TLB(unsigned long addr){
    asm volatile("invlpg (%0)" :: "r" (addr) : "memory");
}

int32_t halt (uint8_t status){
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
    printf("Got to the write syscall\n");
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
    int i;
    for(i=0; i < nbytes && args[i] != '\0' ; i++){
        buf[i] = args[i];
    }

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


int32_t execute (const uint8_t* command){
    //Parse comman into individual arguments
    int spaces[32]={0};         // The index of all the spaces in command
    spaces[0] = 0;

    int num_spaces = 0;         // The number of spaces in command
    int i=0;

    //Loop through command, saving the indexes of all spaces
    for(i=0; command[i]!='\0'; i++){
        if(command[i] == ' '){
            spaces[num_spaces+1] = i;
            num_spaces++;
        }
    }
    printf("%d, %d", num_spaces+1, strlen((char*)command));
    
    spaces[num_spaces+1] = strlen((char*)command);

    //Save args for the getargs call
    int j;
    for(j=spaces[1]; command[j] != '\0'; j++){
        args[j-spaces[1]] = command[j];
    }

    //Check if filename exists

    dentry_t temp_dentry;
    //copy over the filename
    char filename[32] = {'\0'}; //spaces[1]]; 32 is max num of chars

    for(i=0; i<spaces[1]; i++){
        filename[i] = command[i];
    }

    // for(i=0; i<spaces[1]; i++){
    //     putc(filename[i]);
    // }

    //Call read_dentry and check if it can find the function name
    int res = read_dentry_by_name((uint8_t*)filename, &temp_dentry);
    if(res == -1){
        printf("\nFile Does Not Exist ");
        return -1;
    }
    printf("\nfile exists \n");
    
    //Check if the file is an executable
    //Read first 4 bytes
    unsigned char ELF_buf[28];
    res = read_data(temp_dentry.node_num,0,(uint8_t*)ELF_buf,28);
    
    //4 magic numbers to verify if file is an executable
    unsigned char ELF_bits[4] = {127, 69, 76, 70};

    //check if first 4 bytes = elf bits
    for(i=0; i<4; i++){
        printf("%d ", (int)ELF_buf[i]);
        if(ELF_buf[i] != ELF_bits[i]){
            printf("NOT an executable");
            return -1;
        }
    }
    printf("Is an executable\n");

    //Setup Paging
    //Check if we reached max PCBs
    if(PID >= 5){
        printf("Too many PCBs");
        return -1;
    }
    // printf("Plenty of PCBs\n");
    int occupied = 1;
    //Map virtual memory
    for(i = 0; i < 6; i++) // go through each pid
    {
        if(PID_ARRAY[i] == 0) // if PID is free. 
        {
            PID = i;
            PID_ARRAY[i] = 1;
            occupied = 0; // indicates that a pid was not busy.
            break; 
        }
    }
    if(occupied == 1) // if all pids are in use cancel execute. 
    {
        return -1;
    }

    int pd_idx = 32; //The page directory index of virtual address 128MB
    page_directory[pd_idx].present = 1;
    page_directory[pd_idx].read_write = 1;
    page_directory[pd_idx].addr = (CURR_MEM + (PID)*0x400000);
    page_directory[pd_idx].user_supervisor = 1;
    page_directory[pd_idx].page_size = 1;

    do {                                    \
		asm volatile ("call flush_tlb"      \
		);                                  \
	} while (0);

    //Create new PCB
    PCB_t* PCB = Get_PCB_ptr(PID);
    PCB->Active = 1;
    PCB->PID = PID;
    PCB->Parent_PID = 0; //True for 3.3 but not generally

    //Load program from file system into memory directly
    int file_size = (inodes_ptr + temp_dentry.node_num)->length;

    
    res = read_data(temp_dentry.node_num,0,(uint8_t*)0x08048000,file_size); // 0x08048000 is start of program image. 

    //Increment PID

    for(i = 0; i < 8; i++)
    {
        PCB->FDT[i].fop_table_ptr = &null_fop;
        PCB->FDT[i].inode = 0;
        PCB->FDT[i].file_position = 0;
        PCB->FDT[i].flags = 0; 
    }
    PCB->FDT[0].fop_table_ptr = &stdin_fop;
    PCB->FDT[1].fop_table_ptr = &stdout_fop; 
    PCB->FDT[0].flags = 1; 
    PCB->FDT[1].flags = 1;
    strncpy((int8_t*)PCB->syscall_args,(int8_t*)filename,32); // max character length

    tss.ss0 = KERNEL_DS;
    tss.esp0 = 0x800000 - (8192*PID);
    uint32_t EIP = ELF_buf[27] << 24 | ELF_buf[26] << 16 | ELF_buf[25] << 8 | ELF_buf[24];
    uint32_t ESP = 0x8400000 - 4; // 128 MB start of user program? 
    //Save current EBP
    // sti();
    Save_context(ESP,EIP);
    // asm volatile ("                     \n
    //         pushl $0x002B               \n
    //         pushl %%esp                 \n
    //         pushfl                      \n
    //         pushl $0x0010               \n
    //         pushl %[eip_var]                  \n
    //         "
    //         :
    //         : [eip_var]"X"(EIP)
    // );

    //Create Context switch (?)

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

int32_t null_read(int32_t fd, void* buf, int32_t nbytes)
{
    return -1;
}
int32_t null_write(int32_t fd, const void* buf, int32_t nbytes)
{
    return -1;
}
int32_t null_open(const uint8_t* filename)
{
    return -1;
}
int32_t null_close(int32_t fd)
{
    return -1;
}
