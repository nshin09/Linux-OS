#include "syscall.h"
#include "lib.h"
#include "file_system.h"
#include "paging.h"

//Moved these variables to this file cause they weren't being seen properly
const int CURR_MEM = 0x800000; //8 Megabyes
const int MAX_FILE_VALUE = 40000;

int PID = 0;

uint8_t args[128] = {'\0'};

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

    // printf("\n");
    // //Print Args to check splitting at ' '
    // int j;
    // for(j=0; j < num_spaces+1; j++){
    //     for(i=spaces[j]; i < spaces[j+1]; i++){
    //         putc(command[i]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

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
    unsigned char ELF_buf[4];
    res = read_data(temp_dentry.node_num,0,(uint8_t*)ELF_buf,4);
    
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
    printf("Plenty of PCBs\n");

    //Map virtual memory
    int pd_idx = 32; //The page directory index of virtual address 128MB
    page_directory[pd_idx].present = 1;
    page_directory[pd_idx].read_write = 1;
    page_directory[pd_idx].addr = (CURR_MEM + (PID)*0x400000);
    page_directory[pd_idx].user_supervisor = 1;
    page_directory[pd_idx].page_size = 1;

    Flush_TLB(page_directory[pd_idx].addr);

    //Create new PCB
    PCB_t* PCB = Get_PCB_ptr(PID);
    PCB->Active = 1;
    PCB->PID = PID;
    PCB->Parent_PID = 0; //True for 3.3 but not generally

    //Load program from file system into memory directly
    int file_size = (inodes_ptr + temp_dentry.node_num)->length;

    unsigned char Full_buf[MAX_FILE_VALUE];
    res = read_data(temp_dentry.node_num,0,(uint8_t*)Full_buf,file_size);

    memcpy(page_directory[pd_idx].addr, 
        Full_buf, 
        file_size);

    //Increment PID
    PID++;

    //Save current EBP
    asm volatile("pushl %ebp");

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
