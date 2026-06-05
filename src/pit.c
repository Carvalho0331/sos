/* pit.c — Programmable Interval Timer (8253/8254) driver
 *
 * Configures IRQ0 to fire at a chosen frequency and maintains
 * a global tick counter used by pit_sleep().
 */

#include "pit.h"
#include "io.h"

/* Global tick counter, incremented by IRQ0 handler */
volatile unsigned int pit_ticks = 0;

/* Frequency (ticks per second) set by pit_init */
static unsigned int pit_hz = 0;

/* --------------------------------------------------------------- */

/**
 * pit_init:
 * Programmes channel 0 to fire at `hz` interrupts per second.
 * Must be called before STI.
 */
void pit_init(unsigned int hz)
{
    pit_hz   = hz;
    pit_ticks = 0;

    unsigned int divisor = PIT_BASE_FREQ / hz;

    /* Command: channel 0, lobyte/hibyte access, mode 3 (square wave) */
    outb(PIT_COMMAND, 0x36);

    /* Send divisor low byte then high byte */
    outb(PIT_CHANNEL0, (unsigned char)(divisor & 0xFF));
    outb(PIT_CHANNEL0, (unsigned char)((divisor >> 8) & 0xFF));
}

/**
 * pit_get_ticks:
 * Returns current tick count (safe to call from C).
 */
unsigned int pit_get_ticks(void)
{
    return pit_ticks;
}

/**
 * pit_sleep:
 * Busy-waits for at least `ms` milliseconds.
 * Requires pit_init() to have been called and interrupts enabled.
 */
void pit_sleep(unsigned int ms)
{
    if (pit_hz == 0) return;

    unsigned int ticks_needed = (ms * pit_hz) / 1000;
    if (ticks_needed == 0) ticks_needed = 1;

    unsigned int start = pit_ticks;
    while ((pit_ticks - start) < ticks_needed)
        __asm__ volatile ("hlt");
}
