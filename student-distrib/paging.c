#include "paging.h"
#include "paging_asm.h"
#include "lib.h"
#include "x86_desc.h"
// page_table_t page_directory[1024] __attribute__((aligned(4096)));
// page_t page_table[1024] __attribute__((aligned(4096)));
// page_t page_VGA[1024] __attribute__((aligned(4096)));

/* void init_Paging(void)
 * Inputs: void
 * Return Value: void
 * Function: initializes paging by filling in appropriate tables*/
 void initPaging(){
    int i;
    // Fill the page directory with values


    //set up entire directory as not present
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

        page_table[i].present = 0;
        page_table_vidmap[i].present = 0;
        if(i << 12 == 0xB8000) // if the index is equal to the start of VGA memory.
        {
            page_table[i].present = 1;
        }
        page_table[i].available = 0;
        page_table[i].global_page = 0;
        page_table[i].PAT = 0;
        page_table[i].accessed = 0;
        page_table[i].dirty = 0;
        page_table[i].cache_disabled = 0;
        page_table[i].write_through = 0;
        page_table[i].user_supervisor = 0;
        page_table[i].read_write = 1;
        page_table[i].addr = i;

        page_table_vidmap[i].available = 0;
        page_table_vidmap[i].global_page = 0;
        page_table_vidmap[i].PAT = 0;
        page_table_vidmap[i].accessed = 0;
        page_table_vidmap[i].dirty = 0;
        page_table_vidmap[i].cache_disabled = 0;
        page_table_vidmap[i].write_through = 0;
        page_table_vidmap[i].user_supervisor = 0;
        page_table_vidmap[i].read_write = 1;
        page_table_vidmap[i].addr = i;

        
    }

    //set up first 4 MB with present 4 kb pages
    page_directory[0].addr = ((unsigned int)page_table >> 12);;
    page_directory[0].available = 0;
    page_directory[0].global_page = 0;
    page_directory[0].page_size = 0; //0 because 4 kb
    page_directory[0].reserved = 0;
    page_directory[0].accessed = 0;
    page_directory[0].cache_disabled = 0;
    page_directory[0].write_through = 0;
    page_directory[0].user_supervisor = 0;
    page_directory[0].read_write = 1;
    page_directory[0].present = 1; 

    page_directory[1].addr = 0x400000 >> 12; // 0x400000 is the start of kernel. 4 MiB
    page_directory[1].available = 0;
    page_directory[1].global_page = 0;
    page_directory[1].page_size = 1; //1 because 4 mb
    page_directory[1].reserved = 0;
    page_directory[1].accessed = 0;
    page_directory[1].cache_disabled = 0;
    page_directory[1].write_through = 0;
    page_directory[1].user_supervisor = 0;
    page_directory[1].read_write = 1;
    page_directory[1].present = 1; 


    //Create a first page table
    // uint32_t first_page_table[1024] __attribute__((aligned(4096)));
    // //Fill the table
    // for(i = 0; i < 1024; i++){
    //     first_page_table[i] = (i * 0x1000) | 3;
    // }
    // //set the entry in the page_directory
    // page_directory[0].addr = ((unsigned int)first_page_table) | 3;

    // printf("  Enabling paging-");
    // ENABLE PAGING
    // Set CR3 to the base register
    set_PDBR((unsigned int*)page_directory);
    // Enable paging in CR0
    enable_paging();

    // printf("-Paging is enabled  \n");

    // Set up TLB ??
    // Set up MMU ??

}






