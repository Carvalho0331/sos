#include "idt.h"
#include "io.h"

static struct idt_entry idt[256];
static struct idt_ptr ptr;
extern unsigned int interrupt_handlers[256];

void idt_set_gate(unsigned char num, unsigned int base, unsigned short selector, unsigned char flags)
{
    idt[num].base_low  = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].selector  = selector;
    idt[num].zero      = 0;
    idt[num].flags     = flags;
}

void idt_init(void)
{
    ptr.limit = sizeof(idt) - 1;
    ptr.base  = (unsigned int) &idt;

    for (int i = 0; i < 256; i++) {
        if (interrupt_handlers[i] != 0) {
            idt_set_gate(i, interrupt_handlers[i], 0x08, 0x8E);
        }
    }
    __asm__ volatile ("lidt (%0)" : : "r" (&ptr));
}
