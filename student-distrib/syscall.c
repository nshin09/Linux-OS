#include "syscall.h"
#include "syscall_asm.h"
#include "lib.h"
#include "file_system.h"
#include "paging.h"
#include "rtc.h"
#include "terminal.h"
#include "x86_desc.h"
#include "keyboard.h"
//Moved these variables to this file cause they weren't being seen properly
const int CURR_MEM = 0x800000; //8 Megabyes
const int MAX_FILE_VALUE = 40000;
int PID = 0;
int ARGS_SIZE = 128;

int PID_ARRAY[6] = {0,0,0,0,0,0};
uint8_t args[128] = {'\0'};

void Set_PID(int new_PID){
    PID = new_PID;
}

int Get_PID(){
    return PID;
}

int Reserve_PID(int idx){
    if(PID_ARRAY[idx] == 0){
        PID_ARRAY[idx] = 2;
        return 1;
    }
    else{
        return -1;
    }
}

int Open_PID(int idx){
    if(PID_ARRAY[idx] == 2){
        PID_ARRAY[idx] = 0;
        return 1;
    }
    else if(PID_ARRAY[idx] == 0){
        return 0;
    }
    else{
        return -1;
    }
}

/* void: initialize_fop()
 * Inputs: none
 * Return Value: none
 * Function: exactly as name specifies, sets values of all fops to default values, e.g.: null fops get set to corresponding null_values */

void initialize_fop()
{
    //init null fop
    null_fop.open = null_open;
    null_fop.close = null_close;
    null_fop.read = null_read;
    null_fop.write = null_write; 
    //init stdin fop
    stdin_fop.open = &terminal_open;
    stdin_fop.close = &terminal_close;
    stdin_fop.read = &terminal_read;
    stdin_fop.write = &null_write;
    //init stdout fop
    stdout_fop.open = &terminal_open;
    stdout_fop.close = &terminal_close;
    stdout_fop.read = &null_read;
    stdout_fop.write = &terminal_write;
    //init rtc fop
    rtc_fop.open = rtc_open;
    rtc_fop.close = rtc_close;
    rtc_fop.read = rtc_read;
    rtc_fop.write = rtc_write;
    //init directory fop
    directory_fop.open = directory_open;
    directory_fop.close = directory_close;
    directory_fop.read = directory_read;
    directory_fop.write = directory_write;
    //init file fop
    file_fop.open = file_open;
    file_fop.close = file_close;
    file_fop.read = file_read;
    file_fop.write = file_write;
}

/* PCB: Get_PCB_ptr (int local_PID)
 * Inputs: local_PID - value of program index
 * Return Value: PCB
 * Function: returns pointer to process control block pointer depending on whether local process id is present (!-1), or not*/
PCB_t* Get_PCB_ptr (int local_PID){
    if(local_PID == -1) // this is used for file system
    {
        return (PCB_t*)(CURR_MEM - (PID+1)*PCB_size);
    }
    return (PCB_t*)(CURR_MEM - (local_PID+1)*PCB_size);
}
/* void: Flush_TLB(unsigned long addr)
 * Inputs: unsigned long addr
 * Return Value: none
 * Function: clears the values in the transition lookaside buffer, which starts at addr, used in context switching when we need to flush contents of TLBs to prevent misses*/
void Flush_TLB(unsigned long addr){
    asm volatile("invlpg (%0)" :: "r" (addr) : "memory");
}

/* int32: halt (uint8_t status)
 * Inputs: uint8_t status
 * Return Value: 0 if completed successfully
 * Function: terminates a process, returns to parent process. clears pcb, flushes tlb and resets tss, and completes all needed process prior to context switch.*/
int32_t halt (uint8_t status){
    //Clear PCB
    // printf("%d", status);
    PID_ARRAY[PID] = 0;
    //Get PCB
    PCB_t* PCB = Get_PCB_ptr(PID);
    // printf("\nPID: %d     Parent PID: %d\n", PID, PCB->Parent_PID);

    int i;

    for(i=0; i<8; i++){
        close(i);
    }

    //Reset TSS
    tss.ss0 = PCB->SS0;
    tss.esp0 = PCB->ESP0;

    page_directory[32].addr = (CURR_MEM + (PCB->Parent_PID)*0x400000)>> 12;

    Flush_TLB(page_directory[32].addr);
    //Set EBP and ESP to value saved in PCB
    if(PID == PCB->Parent_PID){
        printf("Cannot Exit Base Shell. Restarting\n");
        execute_local((uint8_t*)"shell", 1);
    }

    PID = PCB->Parent_PID;
    Set_EBP_ESP((uint32_t)PCB->EBP, (uint32_t)PCB->ESP);
    return 0;
}
/* int32: read (int32_t fd, void* buf, int32_t nbytes)
 * Inputs: int32_t fd - file descriptor to index into file descriptor table, specifies file 
           void* buf buffer to read into
           int32_t nbytes - num of bytes to read
 * Return Value: nbytes written
 * Return Value: nbytes read
 * Function: reads nbytes from a file and puts in buf .*/
int32_t read (int32_t fd, void* buf, int32_t nbytes){
    // fd: %d, nbytes: %d\n", fd, nbytes);
    //Get File from File Descriptor Table?
    if (fd<0 || fd>MAX_FD_ENTRIES){
        return -1;
    }
    // printf("in read\n");
    PCB_t* PCB = Get_PCB_ptr(PID);
    fdt_entry_t file = PCB->FDT[fd];
    //Call fop table's read
    // printf("fd %d  nbytes %d\n", fd, nbytes);
    int ret = file.fop_table_ptr->read(fd, buf, nbytes);
    // printf("finished read\n");
    return ret;
}
/* int32: write (int32_t fd, void* buf, int32_t nbytes)
 * Inputs: int32_t fd - file descriptor to index into file descriptor table, specifies file 
           void* buf buffer to write into
           int32_t nbytes - num of bytes to write
 * Return Value: nbytes written
 * Function: writes nbytes.*/
int32_t write (int32_t fd, const void* buf, int32_t nbytes){
    //Get File from File Descriptor Table
    // printf("in write\n");
    if (fd<0 || fd>MAX_FD_ENTRIES){
        return -1;
    }

    PCB_t* PCB = Get_PCB_ptr(PID);

    //DEBUG
    // printf("%d %d ", PCB->PID, PCB->Parent_PID);

    fdt_entry_t file = PCB->FDT[fd];
    //Call fop table's write
    int ret = file.fop_table_ptr->write(fd, buf, nbytes);
    // printf("finished write\n");
    
    return ret;
}

/* int32: open (const uint8_t* filename)
 * Inputs: const uint8_t* filename - filename to read from dentry
   Return Value: i, index of MAX_FD_ENTRIES in which we broke in for loop
   Function:: call finds the directory entry corresponding to the named file, allocate an unused file descriptor, and sets up fops for 
   any type of avail file(directory,RTC device, or regular file).
   If the named file does not exist or no descriptors are free, the call returns -1. tldr: provides access to the file system
.*/
int32_t open (const uint8_t* filename){
    //get current pcb
    // printf("starting open\n");
    if(strlen((int8_t*)filename) <= 0 || strlen((int8_t*)filename) > 32){
        return -1;
    }
    int canOpen = 0;
    dentry_t temp_dentry;
    //check if filename exists
    int does_exist = read_dentry_by_name(filename, &temp_dentry);
    if(does_exist ==-1){
        return -1;
    }

    PCB_t* PCB = Get_PCB_ptr(PID);

    //find an unused spot in PCB's file descriptor table
    int i;
    for(i=0;i<MAX_FD_ENTRIES;i++){
        if (PCB->FDT[i].flags == 0){
            canOpen = 1;
            PCB->FDT[i].flags = 1;
            PCB->FDT[i].inode = temp_dentry.node_num;
            // printf("%d", temp_dentry.node_num);
            PCB->FDT[i].file_position = 0;

            // 0 - rtc 1 -dir 2-file
            int fileType = temp_dentry.file_type;
            if (fileType == 0){
                PCB->FDT[i].fop_table_ptr = &rtc_fop;
                Terminals[ScheduledTerminal].rtc_mode = 1; 
                Terminals[ScheduledTerminal].rtc_rate = 1; // 512 represents a frequency of 2 hertz. 
                Terminals[ScheduledTerminal].rtc_ticks = Terminals[ScheduledTerminal].rtc_rate;           
            }
            if (fileType == 1){
                PCB->FDT[i].fop_table_ptr = &directory_fop;
                Terminals[ScheduledTerminal].rtc_mode = 0;   
            }
            if (fileType == 2){
                PCB->FDT[i].fop_table_ptr = &file_fop;
               Terminals[ScheduledTerminal].rtc_mode = 0;    
            }          
            break;
        }
    }

    if(canOpen == 0){
        return -1;
    }

    return i;
}

/* int32: close (int32_t fd)
 * Inputs: int32_t fd - file descriptor to index into file descriptor table, specifies file
   Return Value: 0 if successful, -1 else
   Function:: closes the specified file descriptor and makes it available for return from later calls to open. -1 if trying to close invalid file. sets pcb->fdt values to 0, or 
   defaults.
.*/
int32_t close (int32_t fd){
    //Get File from File Descriptor Table
    //file = fdt[fd]
    PCB_t* PCB = Get_PCB_ptr(PID);
    //check bounds of fd and make sure not to overwrite stdin and stdout
    if(fd<0 || fd == 0 || fd == 1 || fd > MAX_FD_ENTRIES){
        return -1;
    }
    if (PCB->FDT[fd].flags == 0){
        return -1;
    }
    PCB->FDT[fd].flags = 0;
    PCB->FDT[fd].inode = 0;
    PCB->FDT[fd].file_position = 0;
    int closed = PCB->FDT[fd].fop_table_ptr->close(fd);
    PCB->FDT[fd].fop_table_ptr = &null_fop;
    Terminals[ScheduledTerminal].rtc_mode = 0; 
    return closed;
}

/* int32: getargs (uint8_t* buf, int32_t nbytes)
 * Inputs: uint8_t* buf - buffer to write into, int32_t nbytes - num of bytes to write
   Return Value: 0 if successful, or if the arguments and a terminal NULL (0-byte) do not fit in the buffer, returns -1
   Function: parses program's line command into arguments and loads into buf.
.*/
int32_t getargs (uint8_t* buf, int32_t nbytes){
    int i;
    int ret = 0;
    for(i =0; i < nbytes; i++) // clear buffer before reading a new file. 
    {
        buf[i] = '\0';
    }
    for(i=0; i < nbytes && args[i] != '\0' ; i++){
        buf[i] = args[i];
        // printf("%c\n", ((uint8_t*)buf)[i]);
    }
    if(i == 0) 
    ret = -1;
    return ret;
}

/* int32: vidmap (uint8_t** screen_start)
 * Inputs: uint8_t** screen_start
   Return Value: 0 if successful, else returns -1
   Function: n/a
.*/
int32_t vidmap (uint8_t** screen_start){
    // printf("Called Vidmap: screen start: %d\n", screen_start);

    int addr = (uint32_t)screen_start;
    if ( addr < 0x8000000 || addr > (0x8000000 + 0x400000) || screen_start == NULL)
    {
        return -1;
    }  // check if it is within user memory
     

    // set up virtual address at 136 megabytes
  // start of video memory shifted 12 bits

    page_directory[34].present = 1; // 34 is from 0x8800000 >> 22
    page_directory[34].user_supervisor = 1;
    page_directory[34].page_size = 0;
    page_directory[34].addr =((uint32_t)page_table_vidmap) >> 12;

    page_table_vidmap[0].present = 1;
    page_table_vidmap[0].user_supervisor = 1;
    if(ActiveTerminal == ScheduledTerminal) // need to add for CP 5. 
    {
         page_table_vidmap[0].addr = 0xB8;
    }
    else
    {
        page_table_vidmap[0].addr = 0xB8 + 1 + ScheduledTerminal;
    }
   

    do {                                    \
		asm volatile ("call flush_tlb"      \
		);                                  \
	} while (0);

    *screen_start = (uint8_t*)(0x8800000); // start of virtual memory
    return 0x8800000;
}

//These two system calls are extra credit according to the footer on the assignment
int32_t set_handler (int32_t signum, void* handler_address){
    return 0;
}
int32_t sigreturn (void){
    return 0;
}

int32_t execute (const uint8_t* command){
    return execute_local(command, 0);
}

/* int32: execute (const uint8_t* command)
 * Inputs: const uint8_t* command - space seperated command user enters in to terminal to be executed
   Return Value: 0 if successful, else returns -1
   Function: loads and 'executes' new program, based on input command. hands control of processor to new command. undone by halt.
.*/
int32_t execute_local (const uint8_t* command, uint8_t BaseTerminal){
    cli();
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
    // printf("%d, %d\n", num_spaces+1, strlen((char*)command));
    
    spaces[num_spaces+1] = strlen((char*)command);

    //Save args (not including leading space) for the getargs call
    //Clear buffer
    int j;
    for(j=0; j<ARGS_SIZE; j++){
        args[j] = '\0';
    }
    //Save args
    for(j=spaces[1]+1; command[j] != '\0'; j++){
        args[j-spaces[1]-1] = command[j];
    }

    //Check if filename exists

    dentry_t temp_dentry;
    //copy over the filename
    char filename[32] = {'\0'}; //spaces[1]]; 32 is max num of chars

    for(i=0; i<spaces[1]; i++){
        filename[i] = command[i];
    }


    //Call read_dentry and check if it can find the function name
    int res = read_dentry_by_name((uint8_t*)filename, &temp_dentry);
    if(res == -1){
        return -1;
    }
    
    //Check if the file is an executable
    //Read first 4 bytes
    unsigned char ELF_buf[28];
    res = read_data(temp_dentry.node_num,0,(uint8_t*)ELF_buf,28);
    
    //4 magic numbers to verify if file is an executable
    unsigned char ELF_bits[4] = {127, 69, 76, 70};

    //check if first 4 bytes = elf bits
    for(i=0; i<4; i++){
        // printf("%d ", (int)ELF_buf[i]);
        if(ELF_buf[i] != ELF_bits[i]){
            return -1;
        }
    }

    //Setup Paging
    //Check if we reached max PCBs
    if(PID >= 5){
        printf("No available PIDs\n");
        return -1;
    }
    int possibleParent = -1;
    //Map virtual memory 
    for(i = 0; i < 6; i++) // go through each pid
    {
        if(PID_ARRAY[i] == 0) // if PID is free. 
        {
            possibleParent = PID;   //holds the current pid which would be the parent of the new pid
            PID = i;
            PID_ARRAY[i] = 1;
            break; 
        }
    }
    if(possibleParent == -1) // if all pids are in use cancel execute. 
    {
        printf("No available PIDs\n");
        return -1;
    }

    int pd_idx = 32; //The page directory index of virtual address 128MB

    page_directory[pd_idx].addr = (CURR_MEM + (PID)*0x400000)>> 12;
    page_directory[pd_idx].present = 1;
    page_directory[pd_idx].read_write = 1;
    page_directory[pd_idx].user_supervisor = 1;
    page_directory[pd_idx].page_size = 1;

    do {                                    \
		asm volatile ("call flush_tlb"      \
		);                                  \
	} while (0);

    // flush_tlb();

    //Create new PCB
    PCB_t* PCB = Get_PCB_ptr(PID);
    PCB->Active = 1;
    PCB->PID = PID;
    PCB->Parent_PID = possibleParent; //True for 3.3 but not generally

    if(BaseTerminal == 1){
        PCB->Parent_PID = PCB->PID;
    }
    // printf("%d", PCB->Parent_PID);
    // printf("\nPID: %d     Parent PID: %d\n", PID, PCB->Parent_PID);

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

    PCB->SS0 = tss.ss0;
    PCB->ESP0 = tss.esp0;
    PCB->ESP = Get_ESP();
    PCB->EBP = Get_EBP();

    // tss.ss0 = USER_DS;
    tss.esp0 = 0x800000 - (8192*PID);
    uint32_t EIP = ELF_buf[27] << 24 | ELF_buf[26] << 16 | ELF_buf[25] << 8 | ELF_buf[24];
    // uint32_t ESP = 0x8400000 - 4; // 128 MB start of user program? 
    uint32_t ESP = 0x8400000-4; // 128 MB start of user program? 
    //Save current EBP
    // PCB->EBP = Get_EBP();
    sti();
    Save_context(ESP,EIP);
 

    return 0;
}

/*
 * Null functions below are used to fill the FOP table when the 
 * file shouldn't be able to do this action.
 */
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

