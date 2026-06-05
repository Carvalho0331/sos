#include "keyboard.h"
#include "io.h"
#include "pic.h"

#define KBD_DATA_PORT   0x60
#define KBD_STATUS_PORT 0x64

/* US keyboard scancode -> ASCII
 * 0x0E = Backspace -> '\b'
 * 0x1C = Enter     -> '\n'
 * 0x39 = Space     -> ' '
 */
static const char scancode_to_ascii[128] = {
/*00*/  0,    0,   '1', '2', '3', '4', '5', '6',
/*08*/ '7', '8', '9', '0', '-', '=', '\b',  0,
/*10*/ 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
/*18*/ 'o', 'p', '[', ']', '\n',  0,  'a', 's',
/*20*/ 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
/*28*/ '\'', '`',  0, '\\', 'z', 'x', 'c', 'v',
/*30*/ 'b', 'n', 'm', ',', '.', '/',  0,  '*',
/*38*/  0,  ' ',  0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
        0,   0,   0,   0,   0,   0,   0,   0,
};

unsigned char read_scan_code(void)
{
    while (!(inb(KBD_STATUS_PORT) & 1));
    return inb(KBD_DATA_PORT);
}

unsigned char kbd_poll(void)
{
    if (inb(KBD_STATUS_PORT) & 1)
        return inb(KBD_DATA_PORT);
    return 0;
}

char translate_scancode(unsigned char scancode)
{
    if (scancode & 0x80) return 0;  /* key-release: ignore */
    if (scancode < 128)
        return scancode_to_ascii[scancode];
    return 0;
}

void keyboard_init(void)
{
    /* Flush stale bytes */
    while (inb(KBD_STATUS_PORT) & 1)
        inb(KBD_DATA_PORT);
}
