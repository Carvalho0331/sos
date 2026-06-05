#ifndef GUI_H
#define GUI_H

/* GUI mode: simple VGA-text based graphical interface */

/* Special key codes returned by keyboard in GUI mode */
#define GUI_KEY_NONE  0x00
#define GUI_KEY_UP    0x48   /* scan code for up arrow */
#define GUI_KEY_DOWN  0x50   /* scan code for down arrow */
#define GUI_KEY_LEFT  0x4B
#define GUI_KEY_RIGHT 0x4D
#define GUI_KEY_ENTER 0x1C
#define GUI_KEY_ESC   0x01

/* Number of menu items */
#define GUI_NUM_ITEMS 4

void gui_run(void);

#endif
