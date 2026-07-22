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


void task_create(void (*function)(void), taskState state, taskPriority priority, char[]);//call insert within
void scheduler_run(void);
void task_delay(int);

extern unsigned int global_tick;




#endif