#ifndef SYSCALL_H
#define SYSCALL_H

extern void syscall_handler();

extern void Save_context(uint32_t EIP);

#endif
