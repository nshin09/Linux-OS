#ifndef SYSCALL_H
#define SYSCALL_H

/* Calls the appropriate system called based on the passed number*/
extern void syscall_handler();
extern void flush_tlb();
/* Saves the current context and then switches to a new program's context (Called in Execute)*/
extern void Save_context(uint32_t ESP, uint32_t EIP);

/* Gets register values so they can be read in C */
extern uint32_t Get_EBP();
extern uint32_t Get_ESP();

/* Sets EBP and ESP and switches back to the original context (this func is called in halt)*/
extern void Set_EBP_ESP(uint32_t EBP, uint32_t ESP);

// extern void Set_EBP_ESP(uint32_t EBP, uint32_t ESP);
#endif
