#include "io.h"

#define COM1 0x3F8
#define DATA_PORT(base)        (base)
#define LINE_CMD_PORT(base)    (base + 3)
#define LINE_STATUS_PORT(base) (base + 5)
#define LINE_ENABLE_DLAB 0x80

void serial_configure_baud_rate(unsigned short com, unsigned short divisor)
{
    outb(LINE_CMD_PORT(com), LINE_ENABLE_DLAB);
    outb(DATA_PORT(com), (divisor >> 8) & 0xFF);
    outb(DATA_PORT(com), divisor & 0xFF);
}

void serial_configure_line(unsigned short com)
{
    outb(LINE_CMD_PORT(com), 0x03);
}

void serial_configure_fifo(unsigned short com)
{
    outb(com + 2, 0xC7);
}

void serial_configure_modem(unsigned short com)
{
    outb(com + 4, 0x03);
}

int serial_is_transmit_fifo_empty(unsigned short com)
{
    return inb(LINE_STATUS_PORT(com)) & 0x20;
}

void serial_write_char(unsigned short com, char c)
{
    while (!serial_is_transmit_fifo_empty(com));
    outb(DATA_PORT(com), c);
}

void serial_write(unsigned short com, const char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
        serial_write_char(com, str[i]);
}

void serial_initialize(void)
{
    serial_configure_baud_rate(COM1, 1);   /* 115200 baud */
    serial_configure_line(COM1);
    serial_configure_fifo(COM1);
    serial_configure_modem(COM1);
}
