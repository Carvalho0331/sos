/* shell.c — Interactive kernel shell
 *
 * Processes one character at a time (called from keyboard interrupt handler).
 * Supports Enter to execute, Backspace to delete.
 *
 * Built-in commands:
 *   help      — list commands
 *   clear     — clear screen
 *   ticks     — show PIT tick count
 *   meminfo   — show heap/memory info (placeholder)
 *   tasks     — show task count
 *   gui       — launch graphics mode
 *   reboot    — triple-fault reboot
 */

#include "shell.h"
#include "vga.h"
#include "pit.h"
#include "task.h"
#include "gui.h"

/* Command line buffer */
static char buf[SHELL_BUF_SIZE];
static unsigned int buf_len = 0;

/* ------------------------------------------------------------------ */
/* Utility: simple string compare                                       */
/* ------------------------------------------------------------------ */
static int streq(const char *a, const char *b)
{
    while (*a && *b) {
        if (*a != *b) return 0;
        a++; b++;
    }
    return *a == *b;
}

/* ------------------------------------------------------------------ */
/* Utility: print unsigned int                                          */
/* ------------------------------------------------------------------ */
static void print_uint(unsigned int n)
{
    if (n == 0) { vga_putchar('0'); return; }
    char tmp[12];
    int  len = 0;
    while (n > 0) { tmp[len++] = (char)('0' + n % 10); n /= 10; }
    for (int i = len - 1; i >= 0; i--)
        vga_putchar(tmp[i]);
}

/* ------------------------------------------------------------------ */
/* Command execution                                                    */
/* ------------------------------------------------------------------ */
static void execute(void)
{
    /* Null-terminate */
    buf[buf_len] = '\0';

    if (buf_len == 0) {
        /* empty — just show prompt */
    } else if (streq(buf, "help")) {
        vga_puts("\r\nCommands: help clear ticks meminfo tasks gui reboot\r\n");
    } else if (streq(buf, "clear")) {
        vga_clear(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    } else if (streq(buf, "ticks")) {
        vga_puts("\r\nTicks: ");
        print_uint(pit_get_ticks());
        vga_puts("\r\n");
    } else if (streq(buf, "meminfo")) {
        vga_puts("\r\nHeap: 0x200000 - 0x300000 (1 MB)\r\n");
        vga_puts("Physical pages: 4 MB managed region\r\n");
    } else if (streq(buf, "tasks")) {
        vga_puts("\r\nActive tasks: ");
        print_uint((unsigned int)task_current_id() + 1);
        vga_puts("\r\n");
    } else if (streq(buf, "gui")) {
        vga_puts("\r\n");
        gui_run();        /* blocks until ESC */
        /* After GUI exits, redraw prompt */
        vga_clear(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
        vga_puts("SOS> ");
        buf_len = 0;
        return;
    } else if (streq(buf, "reboot")) {
        /* Triple fault: load null IDT and trigger interrupt */
        __asm__ volatile (
            "lidt (0)\n\t"
            "int $3\n\t"
        );
    } else {
        vga_puts("\r\nUnknown command: ");
        vga_puts(buf);
        vga_puts("\r\n");
    }

    buf_len = 0;
    vga_puts("SOS> ");
}

/* ------------------------------------------------------------------ */
/* Public API                                                           */
/* ------------------------------------------------------------------ */

void shell_init(void)
{
    buf_len = 0;
    vga_puts("SOS> ");
}

void shell_run(void)
{
    /* No-op — the shell is driven by shell_on_key() from IRQ handler */
}

/**
 * shell_on_key:
 * Called by the keyboard interrupt handler for every printable key.
 * c == '\n' triggers command execution.
 * c == '\b' deletes the last character.
 */
void shell_on_key(char c)
{
    if (c == '\n' || c == '\r') {
        execute();
    } else if (c == '\b') {
        if (buf_len > 0) {
            buf_len--;
            vga_putchar('\b');
        }
    } else if (buf_len < SHELL_BUF_SIZE - 1) {
        buf[buf_len++] = c;
        vga_putchar(c);
    }
}
