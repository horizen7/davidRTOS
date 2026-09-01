#ifndef TCB_H
#define TCB_H
#include <stdint.h>

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

typedef struct TCB{
    void (*task_function)(void);

    struct TCB* prev;
    struct TCB* next;
    struct TCB** list_head;

    taskState state;
    taskPriority priority;//1, 2, 3, asending importance
    unsigned int wake_tick;
    char op_name[10];

    uint32_t* stack_pointer;
    unsigned int wait_flags;
    EventWaitMode wait_mode;
} TCB;

#endif