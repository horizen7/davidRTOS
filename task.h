#ifndef TASK_H
#define TASK_H

#include "tcb.h"
#include "synch.h"

void task_delay(int);
void task_yield(void);
void task_create(void (*function)(void), taskPriority priority, char[]);
void delete_task(TCB*);
void delay(void);

// CUSTOM TASKS
void high_task(void);
void delay_task(void);
void yield_task(void);
void mutex_locker(void);
void mutex_unlocker(void);
void mutex_waiter(void);
void sema_waiter(void);
void sema_poster(void);
void quick_task(void);

extern TCB* master_list[];
extern unsigned int taskCount;
extern Mutex test_mutex;
extern Semaphore test_sema;

#endif