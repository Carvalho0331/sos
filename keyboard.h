#ifndef KEYBOARD_H
#define KEYBOARD_H

void keyboard_init(void);
unsigned char read_scan_code(void);
char translate_scancode(unsigned char scancode);

#endif
