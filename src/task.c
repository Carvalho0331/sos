/* task.c — Cooperative multitasking (round-robin)
 *
 * Tasks are switched by explicit task_yield() calls.
 * Context switch saves/restores the stack pointer; all other registers
 * are already saved by the C calling convention.
 *
 * Usage:
 *   task_init();
 *   task_create(my_function);
 *   task_yield();  // from within a running task
 */

#include "task.h"

static struct task tasks[MAX_TASKS];
static int current_task = -1;  /* -1 = idle (kmain context) */
static int num_tasks    =  0;

void task_init(void)
{
    for (int i = 0; i < MAX_TASKS; i++) {
        tasks[i].state = TASK_EMPTY;
        tasks[i].id    = i;
    }
    current_task = -1;
    num_tasks    =  0;
}

/**
 * task_create:
 * Initialise a task to run `func`. The task stack is pre-loaded so
 * that when we first switch to it, it will "return" into func.
 */
int task_create(task_func_t func)
{
    if (num_tasks >= MAX_TASKS) return -1;

    /* Find a free slot */
    int slot = -1;
    for (int i = 0; i < MAX_TASKS; i++) {
        if (tasks[i].state == TASK_EMPTY) { slot = i; break; }
    }
    if (slot < 0) return -1;

    struct task *t = &tasks[slot];

    /* Point esp at top of the task's stack, then pre-push:
       - the task function address (so ret from the switch lands there)
       - space for the "dummy" saved registers (we use a minimal stub) */
    unsigned int *sp = &t->stack[TASK_STACK / 4];  /* top of stack array */
    sp--;
    *sp = (unsigned int)func;   /* return address → func */

    /* Save esp just below the pushed return address */
    t->esp   = (unsigned int)sp;
    t->state = TASK_READY;
    num_tasks++;
    return slot;
}

/**
 * task_yield:
 * Save current ESP, find next READY task, restore its ESP, return into it.
 *
 * The switch is done in inline asm to ensure we control the stack precisely.
 */
void task_yield(void)
{
    if (num_tasks == 0) return;

    /* Find next ready task */
    int next = -1;
    int start = (current_task + 1) % MAX_TASKS;
    for (int i = 0; i < MAX_TASKS; i++) {
        int idx = (start + i) % MAX_TASKS;
        if (tasks[idx].state == TASK_READY) {
            next = idx;
            break;
        }
    }
    if (next < 0 || next == current_task) return;

    int prev = current_task;

    /* Mark states */
    if (prev >= 0) tasks[prev].state = TASK_READY;
    tasks[next].state = TASK_RUNNING;
    current_task = next;

    /* Context switch: save ESP of prev task, load ESP of next task.
       The "call" to task_yield already pushed the return address on the
       stack, so when we reload prev's ESP and ret, execution resumes
       right after this asm block. */
    if (prev >= 0) {
        __asm__ volatile (
            "mov %%esp, %0\n\t"   /* save current esp */
            "mov %1, %%esp\n\t"   /* load next esp    */
            "ret\n\t"             /* jump into next task */
            : "=m" (tasks[prev].esp)
            : "m"  (tasks[next].esp)
        );
    } else {
        /* First switch from idle context — just load and jump */
        __asm__ volatile (
            "mov %0, %%esp\n\t"
            "ret\n\t"
            :
            : "m" (tasks[next].esp)
        );
    }
}

int task_current_id(void)
{
    return current_task;
}
