#ifndef SCHEDULER_H
#define SCHEDULER_H

typedef struct TCB
{
    void (*task_function)(void);

    struct TCB* prev;
    struct TCB* next;

    int state;
    int priority;//1, 2, 3, asending importance
    unsigned int wake_tick;
    char op_name[10];
} TCB;

typedef enum
{
    READY,
    RUNNING,
    BLOCKED,
    TERMINATED
} taskState;

typedef enum
{
    LOW,
    MEDIUM,
    HIGH
} taskPriority;

void scheduler_run(void);
TCB* get_current(void);
void set_current(TCB* setter);

extern unsigned int global_tick;

#endif