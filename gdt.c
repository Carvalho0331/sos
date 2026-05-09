#include "gdt.h"

static struct gdt_entry gdt[3];
static struct gdt_ptr gp;

extern void gdt_reload_segments(void);

static void gdt_set_entry(int num, unsigned int base, unsigned int limit,
                          unsigned char access, unsigned char gran)
{
    gdt[num].base_low    = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;
    gdt[num].limit_low   = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access      = access;
}

void gdt_init(void)
{
    // Null descriptor (obrigatório)
    gdt_set_entry(0, 0, 0, 0, 0);
    // Kernel code segment: base=0, limit=4GB, access=0x9A (presente, exec/read, DPL=0), gran=0xCF (4KB granularity, 32-bit)
    gdt_set_entry(1, 0, 0xFFFFF, 0x9A, 0xCF);
    // Kernel data segment: base=0, limit=4GB, access=0x92 (presente, read/write, DPL=0), gran=0xCF
    gdt_set_entry(2, 0, 0xFFFFF, 0x92, 0xCF);

    gp.limit = sizeof(gdt) - 1;
    gp.base  = (unsigned int)&gdt;

    // Carregar a GDT com a instrução lgdt
    __asm__ volatile ("lgdt (%0)" : : "r" (&gp));

    // Recarregar os registos de segmento (far jump para cs, e mov para ds, etc)
    gdt_reload_segments();
}
