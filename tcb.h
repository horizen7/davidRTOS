#ifndef TCB_H
#define TCB_H

typedef struct TCB{
    void (*task_function)(void);

    struct TCB* prev;
    struct TCB* next;

    int state;
    int priority;//1, 2, 3, asending importance
    unsigned int wake_tick;
    char op_name[10];

    unsigned int wait_flags;
    EventWaitMode wait_mode;
} TCB;

typedef enum{
    READY,
    RUNNING,
    BLOCKED,
    BLOCKED_MUTEX,
    BLOCKED_SEMAPHORE,
    BLOCKED_EVENT,
    TERMINATED
} taskState;

typedef enum{
    IDLE_PRIORITY,
    LOW,
    MEDIUM,
    HIGH
} taskPriority;

#endif