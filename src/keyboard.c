#include "keyboard.h"
#include "io.h"
#include "pic.h"

#define KBD_DATA_PORT   0x60
#define KBD_STATUS_PORT 0x64

/* Simple US keyboard mapping (scancode to ASCII) */
static const char scancode_to_ascii[] = {
    0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,   0,
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0,   0,   'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,   '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0
};

/* Read a byte from the keyboard data port (blocking) */
unsigned char read_scan_code(void)
{
    /* Wait until data available (status register bit 0 = 1) */
    while (!(inb(KBD_STATUS_PORT) & 1));
    return inb(KBD_DATA_PORT);
}

/* Translate scan code to ASCII character (ignores shift/release for now) */
char translate_scancode(unsigned char scancode)
{
    if (scancode < sizeof(scancode_to_ascii))
        return scancode_to_ascii[scancode];
    return 0;
}

/* Initialize keyboard (nothing needed for basic setup) */
void keyboard_init(void)
{
    /* Could set LEDs, typematic rate, etc. */
}
