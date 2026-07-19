#ifndef SCHEDULER_H
#define SCHEDULER_H

typedef struct
{
    void (*task_function)(void);
    int state;
    int priority;//1, 2, 3, asending importance
    int wake_tick;
} TCB;

typedef enum
{
    READY,
    RUNNING,
    BLOCKED
} taskState;

void scheduler_add_task(TCB *task);
void queue_update(void);
void scheduler_run(void);

#endif