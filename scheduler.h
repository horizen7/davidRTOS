#ifndef SCHEDULER_H
#define SCHEDULER_H

typedef struct
{
    void (*task_function)(void);
    int state;
    int priority;//1, 2, 3, asending importance
    unsigned int wake_tick;
    unsigned int interval;
} TCB;

typedef enum
{
    READY,  //0
    RUNNING,//1
    BLOCKED //2
} taskState;

void scheduler_add_task(TCB *task);
void queue_update(void);
void scheduler_run(void);

extern unsigned int tick;

#endif