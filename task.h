#ifndef TASK_H
#define TASK_H

#include "tcb.h"
#include "synch.h"

void task_delay(int);
void task_yield(void);
void task_create(void (*function)(void), taskPriority priority, char[]);
void delete_task(TCB*);
void delay(void);

void high_task(void);
void delay_task(void);
void yield_task(void);
void mutex_owner(void);
void mutex_waiter(void);

extern Mutex test_mutex;

#endif