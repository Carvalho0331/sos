/* gui.c — Simple VGA text-mode GUI
 *
 * Draws a full-screen menu with a highlighted selection bar.
 * Navigation: Up/Down arrows move the selection.
 *             Enter executes the selected item.
 *             ESC returns to the shell.
 *
 * BUG FIX: The GUI polls the keyboard directly via port 0x60.
 * While the GUI is running, IRQ1 must be masked so the interrupt
 * handler does not consume keyboard bytes before kb_read_scan() reads
 * them — otherwise kb_read_scan() would wait forever (hung GUI).
 * gui_run() masks IRQ1 on entry and restores the mask on exit.
 */

#include "gui.h"
#include "vga.h"
#include "io.h"
#include "pic.h"
#include "pit.h"

/* ------------------------------------------------------------------ */
/* PIC helpers — mask/unmask IRQ1 (keyboard)                           */
/* ------------------------------------------------------------------ */
#define PIC1_DATA 0x21

static void irq1_mask(void)
{
    unsigned char mask = inb(PIC1_DATA);
    outb(PIC1_DATA, (unsigned char)(mask | 0x02));   /* set bit 1 */
}

/* ------------------------------------------------------------------ */
/* Keyboard polling helpers                                             */
/* ------------------------------------------------------------------ */
#define KBD_DATA   0x60
#define KBD_STATUS 0x64

/* Read one raw scancode byte (press or release). */
static unsigned char kb_read_scan(void)
{
    while (!(inb(KBD_STATUS) & 1));  /* wait for data-ready (OBF flag) */
    return inb(KBD_DATA);
}

/* Wait for an actual key PRESS, discarding release bytes (bit 7 set)
 * and the 0xE0 extended prefix.  Used by action screens so the Enter
 * key-release left in the buffer after menu selection does not
 * immediately dismiss the content page. */
static void kb_wait_keypress(void)
{
    unsigned char sc;
    do {
        sc = kb_read_scan();
    } while ((sc & 0x80) || sc == 0xE0);
}

/* ------------------------------------------------------------------ */
/* Menu items                                                           */
/* ------------------------------------------------------------------ */
static const char *menu_items[GUI_NUM_ITEMS] = {
    "  [ System Info ]  ",
    "  [ Show Ticks   ]  ",
    "  [ About SOS    ]  ",
    "  [ Exit GUI     ]  "
};

/* ------------------------------------------------------------------ */
/* Drawing helpers                                                      */
/* ------------------------------------------------------------------ */

/* Write a string padded to `width` chars at (row, col) with given colours */
static void gui_draw_str(unsigned int row, unsigned int col,
                         const char *s, unsigned char fg, unsigned char bg,
                         unsigned int width)
{
    unsigned int i = 0;
    for (; s[i] && i < width; i++)
        vga_write_cell(row, col + i, s[i], fg, bg);
    for (; i < width; i++)
        vga_write_cell(row, col + i, ' ', fg, bg);
}

/* Fill an entire row with a colour */
static void gui_fill_row(unsigned int row, unsigned char fg, unsigned char bg)
{
    for (unsigned int c = 0; c < VGA_COLS; c++)
        vga_write_cell(row, c, ' ', fg, bg);
}

static void gui_draw_uint(unsigned int row, unsigned int col,
                          unsigned int n,
                          unsigned char fg, unsigned char bg)
{
    char tmp[12];
    int len = 0;
    if (n == 0) tmp[len++] = '0';
    while (n > 0) { tmp[len++] = (char)('0' + n % 10); n /= 10; }
    for (int i = len - 1; i >= 0; i--) {
        vga_write_cell(row, col, tmp[i], fg, bg);
        col++;
    }
}

/* ------------------------------------------------------------------ */
/* Full-screen redraw                                                   */
/* ------------------------------------------------------------------ */
static void gui_draw(int selected)
{
    /* Title bar */
    gui_fill_row(0, VGA_COLOR_BLACK, VGA_COLOR_CYAN);
    gui_draw_str(0, 28, "  SOS Graphical Mode  ",
                 VGA_COLOR_BLACK, VGA_COLOR_CYAN, 24);

    /* Empty rows */
    for (unsigned int r = 1; r < 4; r++)
        gui_fill_row(r, VGA_COLOR_WHITE, VGA_COLOR_BLACK);

    /* Menu items (centred around row 5) */
    for (int i = 0; i < GUI_NUM_ITEMS; i++) {
        unsigned int row = (unsigned int)(5 + i * 2);
        if (i == selected) {
            gui_fill_row(row, VGA_COLOR_BLACK, VGA_COLOR_WHITE);
            gui_draw_str(row, 28, menu_items[i],
                         VGA_COLOR_BLACK, VGA_COLOR_WHITE, 24);
        } else {
            gui_fill_row(row, VGA_COLOR_WHITE, VGA_COLOR_BLUE);
            gui_draw_str(row, 28, menu_items[i],
                         VGA_COLOR_WHITE, VGA_COLOR_BLUE, 24);
        }
        /* gap row */
        if (row + 1 < VGA_ROWS)
            gui_fill_row(row + 1, VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    }

    /* Fill remaining rows */
    for (unsigned int r = (unsigned int)(5 + GUI_NUM_ITEMS * 2);
         r < VGA_ROWS - 1; r++)
        gui_fill_row(r, VGA_COLOR_WHITE, VGA_COLOR_BLACK);

    /* Status bar */
    gui_fill_row(VGA_ROWS - 1, VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREY);
    gui_draw_str(VGA_ROWS - 1, 0,
                 " Arrows: navigate  Enter: select  ESC: back to shell ",
                 VGA_COLOR_BLACK, VGA_COLOR_LIGHT_GREY, VGA_COLS);
}

/* ------------------------------------------------------------------ */
/* Action for each menu item                                            */
/* ------------------------------------------------------------------ */
static void gui_action_sysinfo(void)
{
    /* Clear the content area and print info */
    for (unsigned int r = 1; r < VGA_ROWS - 1; r++)
        gui_fill_row(r, VGA_COLOR_WHITE, VGA_COLOR_BLACK);

    gui_draw_str(2,  2, "Architecture : x86 (i386)", VGA_COLOR_GREEN, VGA_COLOR_BLACK, 40);
    gui_draw_str(3,  2, "Memory       : 4 MB managed", VGA_COLOR_GREEN, VGA_COLOR_BLACK, 40);
    gui_draw_str(4,  2, "Kernel load  : 0x00100000", VGA_COLOR_GREEN, VGA_COLOR_BLACK, 40);
    gui_draw_str(5,  2, "Heap         : 0x200000-0x2FFFFF", VGA_COLOR_GREEN, VGA_COLOR_BLACK, 40);
    gui_draw_str(6,  2, "VGA          : 0xB8000 (text 80x25)", VGA_COLOR_GREEN, VGA_COLOR_BLACK, 40);
    gui_draw_str(7,  2, "Bootloader   : GRUB (Multiboot)", VGA_COLOR_GREEN, VGA_COLOR_BLACK, 40);
    gui_draw_str(9,  2, "Press any key...", VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK, 20);

    kb_wait_keypress();  /* wait for a real key press, not just Enter release */
}

static void gui_action_ticks(void)
{
    for (unsigned int r = 1; r < VGA_ROWS - 1; r++)
        gui_fill_row(r, VGA_COLOR_WHITE, VGA_COLOR_BLACK);

    /* BUG FIX: "PIT Ticks: " is 11 chars starting at col 2 → occupies
     * cols 2-12.  The number must start at col 14 (one space gap), not
     * col 13 which overlapped the trailing space of the label. */
    gui_draw_str(3, 2, "PIT Ticks: ", VGA_COLOR_CYAN, VGA_COLOR_BLACK, 12);
    gui_draw_uint(3, 14, pit_get_ticks(), VGA_COLOR_WHITE, VGA_COLOR_BLACK);

    gui_draw_str(4, 2, "Uptime approx. (ticks / 100 Hz):",
                 VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK, 35);
    gui_draw_uint(5, 2, pit_get_ticks() / 100, VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    gui_draw_str(5, 10, "seconds", VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK, 8);

    gui_draw_str(7, 2, "Press any key...", VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK, 20);
    kb_wait_keypress();
}

static void gui_action_about(void)
{
    for (unsigned int r = 1; r < VGA_ROWS - 1; r++)
        gui_fill_row(r, VGA_COLOR_WHITE, VGA_COLOR_BLACK);

    gui_draw_str(2, 20, "SOS - Simple Operating System",
                 VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK, 32);
    gui_draw_str(4,  2, "Author : Salimo Carvalho",
                 VGA_COLOR_GREEN, VGA_COLOR_BLACK, 30);
    gui_draw_str(5,  2, "Arch   : x86 freestanding",
                 VGA_COLOR_GREEN, VGA_COLOR_BLACK, 30);
    gui_draw_str(6,  2, "Lang   : C + NASM",
                 VGA_COLOR_GREEN, VGA_COLOR_BLACK, 30);
    gui_draw_str(7,  2, "Boot   : GRUB Multiboot",
                 VGA_COLOR_GREEN, VGA_COLOR_BLACK, 30);
    gui_draw_str(9,  2, "Press any key...",
                 VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK, 20);
    kb_wait_keypress();
}

/* ------------------------------------------------------------------ */
/* Main GUI loop                                                        */
/* ------------------------------------------------------------------ */

/**
 * gui_run:
 * Enters the GUI loop. Returns when the user presses ESC or selects
 * "Exit GUI". Saves and restores VGA state (colour only; cursor is reset
 * after return because the shell calls vga_clear()).
 */
void gui_run(void)
{
    int selected = 0;
    int running  = 1;

    /* BUG FIX: Mask IRQ1 (keyboard) while the GUI is running.
     * Without this, the IRQ1 interrupt handler calls kbd_poll() and
     * consumes the keyboard byte from port 0x60 the moment a key is
     * pressed.  When kb_read_scan() (used throughout this file) then
     * spins waiting for the OBF flag to be set, the byte is already gone
     * and the GUI hangs indefinitely.
     * We restore the original PIC1 mask on exit. */
    unsigned char saved_pic1_mask = inb(PIC1_DATA);
    irq1_mask();

    /* Push any stale byte out of the keyboard buffer */
    if (inb(KBD_STATUS) & 1) inb(KBD_DATA);

    /* Disable hardware cursor visibility while in GUI */
    vga_move_cursor(VGA_ROWS - 1, VGA_COLS - 1);

    gui_draw(selected);

    while (running) {
        unsigned char sc = kb_read_scan();

        /* Ignore key-release events (bit 7 set) */
        if (sc & 0x80) continue;

        /* Extended key prefix: consume the second byte and treat it the
         * same as the bare scancode below.
         * GRUB leaves the keyboard controller in AT-translation mode
         * (scancode set 1).  In that mode many BIOSes and QEMU drop the
         * 0xE0 prefix entirely, so arrow keys arrive as plain 0x48/0x50.
         * By reading past 0xE0 and then falling through to the unified
         * switch we handle both cases correctly. */
        if (sc == 0xE0) {
            sc = kb_read_scan();
            if (sc & 0x80) continue;  /* release of extended key — discard */
        }

        switch (sc) {
            case GUI_KEY_UP:
                selected = (selected - 1 + GUI_NUM_ITEMS) % GUI_NUM_ITEMS;
                gui_draw(selected);
                break;
            case GUI_KEY_DOWN:
                selected = (selected + 1) % GUI_NUM_ITEMS;
                gui_draw(selected);
                break;
            case GUI_KEY_ESC:
                running = 0;
                break;
            case GUI_KEY_ENTER:
                switch (selected) {
                    case 0: gui_action_sysinfo(); break;
                    case 1: gui_action_ticks();   break;
                    case 2: gui_action_about();   break;
                    case 3: running = 0;          break;
                    default: break;
                }
                if (running) gui_draw(selected);
                break;
            default:
                break;
        }
    }

    /* Restore IRQ1 mask and clean screen for caller */
    outb(PIC1_DATA, saved_pic1_mask);

    vga_clear(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
}
