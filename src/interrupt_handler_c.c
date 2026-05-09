#include "pic.h"
#include "keyboard.h"
#include "serial.h"

#define COM1 0x3F8

void write(char *buf, unsigned int len);
void serial_write(unsigned short com, const char *str);

void interrupt_handler_c(int interrupt, int error_code)
{
    (void)error_code;   /* silencia o aviso de parâmetro não usado */

    /* Acknowledge hardware interrupts */
    if (interrupt >= 0x20 && interrupt < 0x30) {
        pic_acknowledge(interrupt);
    }

    /* Keyboard interrupt (IRQ1 remapped to 0x21) */
    if (interrupt == 0x21) {
        unsigned char scancode = read_scan_code();
        char c = translate_scancode(scancode);
        if (c) {
            char buf[2] = {c, '\0'};
            write(buf, 1);
            serial_write(COM1, buf);
        }
    }
}
