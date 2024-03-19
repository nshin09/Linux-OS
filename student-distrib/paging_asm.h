#ifndef PAGING_ASM_H
#define PAGING_ASM_H

//Sets the Base Register of the page directory
extern void set_PDBR(unsigned int*);

//Enables paging functionality
extern void enable_paging();

#endif

