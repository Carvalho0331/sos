#ifndef SYSCALL_H
#define SYSCALL_H

/* Syscall numbers (INT 0x80) */
#define SYS_WRITE  1
#define SYS_EXIT   2
#define SYS_GETKEY 3

void syscall_init(void);
int  syscall_handler(int number, int arg1, int arg2, int arg3);

#endif
