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
    unsigned int time_slice_remaining;
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
static void insert_task(TCB* task);
static void remove_task(TCB* task);
static void insert_ready(TCB* task);
static void insert_blocked(TCB* task);
static void remove_ready(TCB* task);
static void remove_blocked(TCB* task);
void task_delay(int);

extern unsigned int global_tick;
static void tick();
static void update_blocked();



#endif