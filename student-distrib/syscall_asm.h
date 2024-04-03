#ifndef SYSCALL_H
#define SYSCALL_H

extern void syscall_handler();

extern void Save_context(uint32_t ESP, uint32_t EIP);

extern uint32_t Get_EBP();
extern void Set_EBP_ESP(uint32_t EBP);

#endif
