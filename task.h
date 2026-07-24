#ifndef TASK_H
#define TASK_H

void task_delay(int);
void task_yield(void);
void task_create(void (*function)(void), taskPriority priority, char[]);
void delete_task(TCB*);
void delay(void);

void down(void);

void hello(void);

void count(void);

void list(void);

void point(void);

#endif