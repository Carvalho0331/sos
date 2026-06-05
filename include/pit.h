#ifndef PIT_H
#define PIT_H

/* PIT I/O ports */
#define PIT_CHANNEL0  0x40
#define PIT_COMMAND   0x43
#define PIT_BASE_FREQ 1193182  /* Hz */

/* Global tick counter (incremented by IRQ0 handler) */
extern volatile unsigned int pit_ticks;

void pit_init(unsigned int hz);
void pit_sleep(unsigned int ms);
unsigned int pit_get_ticks(void);

#endif
