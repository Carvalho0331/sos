#include "io.h"

/* Framebuffer base address (video memory in text mode) */
#define FB 0xB8000

/* VGA text mode colors */
#define FB_GREEN     2
#define FB_DARK_GREY 8

/* VGA cursor I/O ports */
#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT    0x3D5

/* Cursor commands */
#define FB_HIGH_BYTE    14
#define FB_LOW_BYTE     15

/* Pointer to video memory */
char *fb = (char*) FB;

/**
 * fb_write_cell:
 * Writes a character with foreground and background color
 * to a specific position in the framebuffer.
 *
 * @param i   Position index (character cell, not byte index)
 * @param c   Character to display
 * @param fg  Foreground color
 * @param bg  Background color
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    fb[i * 2]     = c;                              /* ASCII character */
    fb[i * 2 + 1] = (fg << 4) | (bg & 0x0F);       /* Color byte */
}

/**
 * fb_move_cursor:
 * Moves the hardware cursor to the given position.
 *
 * The cursor position is a 16-bit value:
 * row * 80 + column
 *
 * @param pos Cursor position in text mode
 */
void fb_move_cursor(unsigned short pos)
{
    /* Send high byte of cursor position */
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE);
    outb(FB_DATA_PORT, (pos >> 8) & 0xFF);

    /* Send low byte of cursor position */
    outb(FB_COMMAND_PORT, FB_LOW_BYTE);
    outb(FB_DATA_PORT, pos & 0xFF);
}

/**
 * write:
 * Writes len bytes from buf to the screen, advancing the cursor.
 * (Simplified version: no newline handling, no scroll)
 */
int write(char *buf, unsigned int len)
{
    static unsigned int cursor_pos = 0;
    for (unsigned int i = 0; i < len; i++) {
        fb_write_cell(cursor_pos, buf[i], FB_GREEN, FB_DARK_GREY);
        cursor_pos++;
    }
    fb_move_cursor(cursor_pos);
    return len;
}

/**
 * Kernel entry point
 */
void kmain(void)
{
    write("Welcome to SOS", 15);
    write("Hello sir Carvalho", 18);
}
