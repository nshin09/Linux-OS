#include "paging.h"
#include "paging_asm.h"
#include "lib.h"

page_table_t page_directory[1024] __attribute__((aligned(4096)));

void initPaging(){
    int i;
    // Fill the page directory with values
    for(i = 0; i < 1024; i++){
        page_directory[i].addr = 0; //= 0x00000002; //To use this line, make it a uint32_t struct above and in paging.h
        page_directory[i].available = 0;
        page_directory[i].global_page = 0;
        page_directory[i].page_size = 0;
        page_directory[i].reserved = 0;
        page_directory[i].accessed = 0;
        page_directory[i].cache_disabled = 0;
        page_directory[i].write_through = 0;
        page_directory[i].user_supervisor = 0;
        page_directory[i].read_write = 1;
        page_directory[i].present = 0; 
    }

    //Create a first page table
    uint32_t first_page_table[1024] __attribute__((aligned(4096)));
    //Fill the table
    for(i = 0; i < 1024; i++){
        first_page_table[i] = (i * 0x1000) | 3;
    }
    //set the entry in the page_directory
    page_directory[0].addr = ((unsigned int)first_page_table) | 3;

    printf("  Enabling paging-");
    // ENABLE PAGING
    // Set CR3 to the base register
    set_PDBR((unsigned int)page_directory);
    // Enable paging in CR0
    enable_paging();

    printf("-Paging is enabled  \n");

    // Set up TLB ??
    // Set up MMU ??

}

void createVirtualAddressSpace(){
    // Set up new page directory

    // I don't know if this function is needed. 
    // I'm taking it from Appendix C in the assignment
    // but I have no clue when it would be called.
}




