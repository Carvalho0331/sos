/* vga.c — Full VGA text-mode driver
 *
 * Manages a 80×25 text-mode framebuffer at 0xB8000.
 * Supports scrolling, colour selection, and hardware cursor.
 */

#include "vga.h"
#include "io.h"

/* Video memory base */
static char *fb = (char *) 0xB8000;

/* Current cursor position */
static unsigned int vga_row = 0;
static unsigned int vga_col = 0;

/* Current colour byte (fg | bg<<4) — default green on black */
static unsigned char vga_color = (VGA_COLOR_GREEN) | (VGA_COLOR_BLACK << 4);

/* ------------------------------------------------------------------ */

/* Update the hardware cursor to match (vga_row, vga_col). */
static void hw_cursor_update(void)
{
    unsigned short pos = (unsigned short)(vga_row * VGA_COLS + vga_col);
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE);
    outb(FB_DATA_PORT,    (pos >> 8) & 0xFF);
    outb(FB_COMMAND_PORT, FB_LOW_BYTE);
    outb(FB_DATA_PORT,     pos & 0xFF);
}

/* Scroll the screen up by one line. */
static void vga_scroll(void)
{
    /* Move every line one row up */
    for (int row = 1; row < VGA_ROWS; row++) {
        for (int col = 0; col < VGA_COLS; col++) {
            int dst = ((row - 1) * VGA_COLS + col) * 2;
            int src =  (row      * VGA_COLS + col) * 2;
            fb[dst]     = fb[src];
            fb[dst + 1] = fb[src + 1];
        }
    }
    /* Clear last row */
    int last = (VGA_ROWS - 1) * VGA_COLS;
    for (int col = 0; col < VGA_COLS; col++) {
        fb[(last + col) * 2]     = ' ';
        fb[(last + col) * 2 + 1] = vga_color;
    }
    vga_row = VGA_ROWS - 1;
    vga_col = 0;
}

/* ------------------------------------------------------------------ */

void vga_init(void)
{
    vga_row   = 0;
    vga_col   = 0;
    vga_color = VGA_COLOR_GREEN | (VGA_COLOR_BLACK << 4);
    vga_clear(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
}

void vga_clear(unsigned char fg, unsigned char bg)
{
    unsigned char col_byte = (unsigned char)(fg | (bg << 4));
    for (int i = 0; i < VGA_ROWS * VGA_COLS; i++) {
        fb[i * 2]     = ' ';
        fb[i * 2 + 1] = col_byte;
    }
    vga_row   = 0;
    vga_col   = 0;
    vga_color = col_byte;
    hw_cursor_update();
}

void vga_set_color(unsigned char fg, unsigned char bg)
{
    vga_color = (unsigned char)(fg | (bg << 4));
}

void vga_write_cell(unsigned int row, unsigned int col,
                    char c, unsigned char fg, unsigned char bg)
{
    if (row >= VGA_ROWS || col >= VGA_COLS) return;
    int idx = (int)((row * VGA_COLS + col) * 2);
    fb[idx]     = c;
    fb[idx + 1] = (unsigned char)(fg | (bg << 4));
}

void vga_move_cursor(unsigned int row, unsigned int col)
{
    if (row >= VGA_ROWS) row = VGA_ROWS - 1;
    if (col >= VGA_COLS) col = VGA_COLS - 1;
    vga_row = row;
    vga_col = col;
    hw_cursor_update();
}

void vga_putchar(char c)
{
    if (c == '\n') {
        vga_col = 0;
        vga_row++;
        if (vga_row >= VGA_ROWS)
            vga_scroll();
        hw_cursor_update();
        return;
    }
    if (c == '\r') {
        vga_col = 0;
        hw_cursor_update();
        return;
    }
    if (c == '\b') {
        if (vga_col > 0) {
            vga_col--;
            vga_write_cell(vga_row, vga_col, ' ', VGA_COLOR_GREEN, VGA_COLOR_BLACK);
            hw_cursor_update();
        }
        return;
    }

    vga_write_cell(vga_row, vga_col, c,
                   vga_color & 0x0F, (vga_color >> 4) & 0x0F);
    vga_col++;
    if (vga_col >= VGA_COLS) {
        vga_col = 0;
        vga_row++;
        if (vga_row >= VGA_ROWS)
            vga_scroll();
    }
    hw_cursor_update();
}

void vga_puts(const char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
        vga_putchar(str[i]);
}

unsigned int vga_get_row(void) { return vga_row; }
unsigned int vga_get_col(void) { return vga_col; }
