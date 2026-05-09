#include "pic.h"
#include "io.h"

#define PIC1_CMD  0x20
#define PIC1_DATA 0x21
#define PIC2_CMD  0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI   0x20
#define ICW1_ICW4 0x01
#define ICW1_INIT 0x10
#define ICW4_8086 0x01

static void io_wait(void) { outb(0x80, 0); }

void pic_remap(void)
{
    unsigned char mask1 = inb(PIC1_DATA);
    unsigned char mask2 = inb(PIC2_DATA);

    outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, 0x20);
    io_wait();
    outb(PIC2_DATA, 0x28);
    io_wait();

    outb(PIC1_DATA, 0x04);
    io_wait();
    outb(PIC2_DATA, 0x02);
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, mask1);
    outb(PIC2_DATA, mask2);
}

void pic_mask(unsigned char mask1, unsigned char mask2)
{
    outb(PIC1_DATA, mask1);
    outb(PIC2_DATA, mask2);
}

void pic_acknowledge(unsigned char interrupt)
{
    if (interrupt >= 0x20 && interrupt < 0x28)
        outb(PIC1_CMD, PIC_EOI);
    else if (interrupt >= 0x28 && interrupt < 0x30)
        outb(PIC2_CMD, PIC_EOI);
}
