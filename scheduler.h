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
    unsigned int interval;
} TCB;

typedef enum
{
    READY,  //0
    RUNNING,//1
    BLOCKED //2
} taskState;

typedef enum
{
    LOW,
    MEDIUM,
    HIGH
} taskPriority;


void task_insert(TCB *task);
void create_tcb(TCB** tail, void (*function)(void), taskState state, taskPriority priority, unsigned int interval);//call insert within
void pop_head(TCB** head);
void queue_update(void);
void scheduler_run(void);

extern unsigned int tick;

#endif