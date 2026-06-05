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
    /* BUG FIX: Do NOT save/restore old BIOS masks — the BIOS leaves all
     * IRQs masked (0xFF/0xFF).  Restoring those masks after remap would
     * silently block every interrupt, including IRQ0 (timer) and IRQ1
     * (keyboard).  We leave both PICs fully masked here and let kmain
     * call pic_mask() with the desired mask immediately after. */

    outb(PIC1_CMD, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_CMD, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, 0x20);   /* PIC1 vector offset → IRQ0-7 = INT 0x20-0x27 */
    io_wait();
    outb(PIC2_DATA, 0x28);   /* PIC2 vector offset → IRQ8-15 = INT 0x28-0x2F */
    io_wait();

    outb(PIC1_DATA, 0x04);   /* tell PIC1 there is a slave at IRQ2 */
    io_wait();
    outb(PIC2_DATA, 0x02);   /* tell PIC2 its cascade identity */
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    /* Leave all IRQs masked; kmain will unmask what it needs. */
    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);
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
