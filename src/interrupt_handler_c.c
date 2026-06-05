/* interrupt_handler_c.c — C-level interrupt dispatcher
 *
 * Called from the common ASM trampoline in asm/interrupt.s.
 * Handles:
 *   IRQ0  (0x20) → PIT tick counter
 *   IRQ1  (0x21) → Keyboard → shell
 *   Others       → silently acknowledged
 *
 * NOTE: keyboard is also polled in the main idle loop (kmain.c)
 * as a fallback for environments where IRQ1 is unreliable (e.g. WSL/QEMU).
 */

#include "pic.h"
#include "keyboard.h"
#include "pit.h"
#include "shell.h"

void interrupt_handler_c(int interrupt, int error_code)
{
    (void)error_code;

    /* Acknowledge hardware interrupts before processing */
    if (interrupt >= 0x20 && interrupt < 0x30)
        pic_acknowledge(interrupt);

    /* IRQ0 → PIT timer tick */
    if (interrupt == 0x20) {
        pit_ticks++;
        return;
    }

    /* IRQ1 → keyboard (non-blocking read — data is already ready) */
    if (interrupt == 0x21) {
        unsigned char scancode = kbd_poll();
        if (scancode) {
            char c = translate_scancode(scancode);
            if (c) shell_on_key(c);
        }
    }
}
