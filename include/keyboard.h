#ifndef KEYBOARD_H
#define KEYBOARD_H

void keyboard_init(void);
unsigned char read_scan_code(void);
unsigned char kbd_poll(void);
char translate_scancode(unsigned char scancode);

#endif
