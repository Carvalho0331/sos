/* kmain.c — Kernel entry point
 *
 * Initialisation order:
 *   GDT → IDT → PIC → PIT → Memory Manager → Paging → Syscalls →
 *   Keyboard → VGA → Serial → Shell → STI → idle loop
 */

#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "pit.h"
#include "mm.h"
#include "paging.h"
#include "syscall.h"
#include "keyboard.h"
#include "vga.h"
#include "serial.h"
#include "shell.h"

#define COM1 0x3F8

void kmain(void)
{
    /* -------- Processor structures -------- */
    gdt_init();
    idt_init();

    /* -------- Interrupt controller -------- */
    pic_remap();
    /* Enable IRQ0 (PIT) and IRQ1 (keyboard); mask all others */
    pic_mask(0xFC, 0xFF);   /* 0b11111100: unmask IRQ0 + IRQ1 */

    /* -------- PIT timer (100 Hz) -------- */
    pit_init(100);

    /* -------- Memory management -------- */
    mm_init();

    /* -------- Paging (identity map 0–4 MB, disabled by default) -------- */
    paging_init();
    /* paging_enable(); */   /* uncomment to activate paging */

    /* -------- Syscall interface -------- */
    syscall_init();

    /* -------- Drivers -------- */
    keyboard_init();
    serial_initialize();

    /* -------- Display -------- */
    vga_init();
    vga_set_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);

    /* -------- Welcome banner -------- */
    vga_puts("=======================================\n");
    vga_puts("   SOS - Simple Operating System\n");
    vga_puts("   Author: Salimo Carvalho\n");
    vga_puts("=======================================\n");
    vga_puts("\n");

    serial_write(COM1, "[OK] SOS kernel started\n");
    serial_write(COM1, "[OK] GDT, IDT, PIC, PIT, MM, Paging, Syscall ready\n");

    /* -------- Shell -------- */
    shell_init();

    /* -------- Enable interrupts -------- */
    __asm__ volatile ("sti");

    /* -------- Main loop: HLT until interrupt --------
     *
     * BUG FIX: The original code polled kbd_poll() here AND let the IRQ1
     * handler call shell_on_key() — a single keystroke was processed TWICE
     * (double characters on screen).
     *
     * Shell input is now driven exclusively by the IRQ1 interrupt handler
     * in interrupt_handler_c.c.  This idle loop simply halts the CPU
     * between interrupts.
     */
    while (1) {
        __asm__ volatile ("hlt");
    }
}
