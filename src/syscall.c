/* syscall.c — Minimal INT 0x80 syscall interface
 *
 * Registered as IDT gate 0x80 (trap gate, ring 0 for now).
 * User programs invoke with:
 *   mov eax, <number>
 *   mov ebx, <arg1>
 *   mov ecx, <arg2>
 *   mov edx, <arg3>
 *   int 0x80
 *
 * Supported numbers:
 *   1  SYS_WRITE  — write arg1 chars from address arg2 to VGA
 *   2  SYS_EXIT   — halt the kernel (hang)
 *   3  SYS_GETKEY — return last key in arg1 buffer (not yet wired)
 */

#include "syscall.h"
#include "idt.h"
#include "vga.h"

/* The raw stub is in asm/syscall_stub.s; it calls this C function */
int syscall_handler(int number, int arg1, int arg2, int arg3)
{
    (void)arg3;

    switch (number) {
        case SYS_WRITE: {
            /* arg1 = length, arg2 = pointer to string */
            const char *s = (const char *)(unsigned int)arg2;
            int len       = arg1;
            for (int i = 0; i < len && s[i]; i++)
                vga_putchar(s[i]);
            return len;
        }
        case SYS_EXIT:
            /* Hang — no process model yet */
            __asm__ volatile ("cli; hlt");
            return 0;
        case SYS_GETKEY:
            return 0; /* placeholder */
        default:
            return -1;
    }
}

void syscall_init(void)
{
    /* Gate 0x80: present, DPL=0, 32-bit interrupt gate (0x8E) */
    extern void syscall_stub(void);
    idt_set_gate(0x80, (unsigned int)syscall_stub, 0x08, 0x8E);
}
