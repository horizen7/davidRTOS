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


void create_tcb(void (*function)(void), taskState state, taskPriority priority, unsigned int interval);//call insert within
void scheduler_run(void);

static void pop_head(TCB** head);
static void queue_update(void);

static void task_insert(TCB* task);
static void task_remove(TCB* task);
static void ready_insert(TCB* task);
static void blocked_insert(TCB* task);
static void ready_remove(TCB* task);
static void blocked_remove(TCB* task);

extern unsigned int tick;

#endif