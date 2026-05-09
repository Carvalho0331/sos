#ifndef SERIAL_H
#define SERIAL_H

#define COM1 0x3F8

void serial_initialize(void);
void serial_write(unsigned short com, const char *str);

#endif
