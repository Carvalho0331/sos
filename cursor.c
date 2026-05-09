#include "io.h"

/* Ports do framebuffer */
#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT    0x3D5

/* Comandos do cursor */
#define FB_HIGH_BYTE    14
#define FB_LOW_BYTE     15

void fb_move_cursor(unsigned short pos)
{
    /* enviar byte alto */
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE);
    outb(FB_DATA_PORT, (pos >> 8) & 0xFF);

    /* enviar byte baixo */
    outb(FB_COMMAND_PORT, FB_LOW_BYTE);
    outb(FB_DATA_PORT, pos & 0xFF);
}
