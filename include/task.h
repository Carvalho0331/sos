#ifndef TASK_H
#define TASK_H

/* Maximum number of concurrent tasks */
#define MAX_TASKS   8
/* Each task stack size */
#define TASK_STACK  4096

/* Task states */
#define TASK_EMPTY   0
#define TASK_READY   1
#define TASK_RUNNING 2

typedef void (*task_func_t)(void);

struct task {
    unsigned int esp;          /* saved stack pointer */
    unsigned int stack[TASK_STACK / 4];
    int          state;
    int          id;
};

void task_init(void);
int  task_create(task_func_t func);
void task_yield(void);          /* cooperative switch to next ready task */
int  task_current_id(void);

#endif
