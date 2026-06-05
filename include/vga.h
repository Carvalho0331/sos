#ifndef VGA_H
#define VGA_H

/* VGA text mode dimensions */
#define VGA_COLS  80
#define VGA_ROWS  25

/* VGA color definitions */
#define VGA_COLOR_BLACK         0
#define VGA_COLOR_BLUE          1
#define VGA_COLOR_GREEN         2
#define VGA_COLOR_CYAN          3
#define VGA_COLOR_RED           4
#define VGA_COLOR_MAGENTA       5
#define VGA_COLOR_BROWN         6
#define VGA_COLOR_LIGHT_GREY    7
#define VGA_COLOR_DARK_GREY     8
#define VGA_COLOR_LIGHT_BLUE    9
#define VGA_COLOR_LIGHT_GREEN   10
#define VGA_COLOR_LIGHT_CYAN    11
#define VGA_COLOR_LIGHT_RED     12
#define VGA_COLOR_LIGHT_MAGENTA 13
#define VGA_COLOR_LIGHT_BROWN   14
#define VGA_COLOR_WHITE         15

/* I/O ports */
#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT    0x3D5
#define FB_HIGH_BYTE    14
#define FB_LOW_BYTE     15

void vga_init(void);
void vga_clear(unsigned char fg, unsigned char bg);
void vga_set_color(unsigned char fg, unsigned char bg);
void vga_putchar(char c);
void vga_puts(const char *str);
void vga_move_cursor(unsigned int row, unsigned int col);
void vga_write_cell(unsigned int row, unsigned int col,
                    char c, unsigned char fg, unsigned char bg);
unsigned int vga_get_row(void);
unsigned int vga_get_col(void);

#endif
