#ifndef SHELL_H
#define SHELL_H

/* Maximum command line length */
#define SHELL_BUF_SIZE 128

void shell_init(void);
void shell_run(void);
void shell_on_key(char c);

#endif
