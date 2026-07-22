#include "scheduler.h"
#include "task.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int taskCount, global_tick = 0;

TCB* master_list[MAX_TASKS];
static TCB* ready_head = NULL;
static TCB* blocked_head = NULL;
static TCB* current_tcb = NULL;

static void tick();
static void update_blocked();
static void insert_task(TCB* task);
static void remove_task(TCB* task);
static void insert_ready(TCB* task);
static void insert_blocked(TCB* task);
static void remove_ready(TCB* task);
static void remove_blocked(TCB* task);


void task_create(void (*function)(void), taskState state, taskPriority priority, char name[])
{
    TCB* new_tcb = malloc(sizeof(TCB));
    if(new_tcb == NULL){
        printf("\n### Error: unable to allocate memory. ###\n");
        return;
    }
    *new_tcb = (TCB){
        .task_function = function,
        .state = state,
        .priority = priority,
        .wake_tick = 0,
        .next = NULL,
        .prev = NULL
    };
    strcpy(new_tcb->op_name, name);
    if(taskCount < MAX_TASKS){
        master_list[taskCount] = new_tcb;
        taskCount++;
    }
    else{
        printf("\n### Error: max task count reached. ###\n");
        return;
    }
    insert_task(new_tcb);
}

static void insert_task(TCB* task)
{
    switch(task->state){
        case READY:
        insert_ready(task);
        break;
        case BLOCKED:
        insert_blocked(task);
        break;
        case RUNNING:
        printf("\n### Error: tried inserting running task. ###\n");
        break;
    }
}

static void remove_task(TCB* task)
{
    switch(task->state){
        case READY:
        remove_ready(task);
        break;
        case BLOCKED:
        remove_blocked(task);
        break;
        case RUNNING:
        printf("\n### Error: tried removing running task. ###\n");
        break;
    }
}

static void insert_ready(TCB* task)
{
    task->state = READY;
    if(ready_head == NULL){
        ready_head = task;
        task->prev = NULL;
        task->next = NULL;
        return;
    }
    if(task->priority > ready_head->priority){
        ready_head->prev = task;
        task->next = ready_head;
        task->prev = NULL;
        ready_head = task;
        return;
    }
    TCB* current = ready_head;
    TCB* previous = NULL;
    while(current != NULL && current->priority >= task->priority){
        previous = current;
        current = current->next;
    }
    previous->next = task;
    task->prev = previous;
    task->next = current;

    if(current != NULL){
        current->prev = task;
    }
}

static void insert_blocked(TCB* task)
{
    task->state = BLOCKED;
    if(blocked_head == NULL){
        blocked_head = task;
        task->prev = NULL;
        task->next = NULL;
        return;
    }
    if(task->wake_tick < blocked_head->wake_tick){
        blocked_head->prev = task;
        task->next = blocked_head;
        task->prev = NULL;
        blocked_head = task;
        return;
    }
    TCB* current = blocked_head;
    TCB* previous = NULL;
    while(current != NULL && current->wake_tick <= task->wake_tick){
        previous = current;
        current = current->next;
    }
    previous->next = task;
    task->prev = previous;
    task->next = current;

    if(current != NULL){
        current->prev = task;
    }

}
static void remove_ready(TCB* task)
{
    if(task == NULL){
        printf("\n### Error: trying to work with nullpointer. ###\n");
        return;
    }
    if(task->prev != NULL){
        task->prev->next = task->next;
    }
    else{
        ready_head = task->next;
    }
    if(task->next != NULL){
        task->next->prev = task->prev;
    }
    
    task->next = NULL;
    task->prev = NULL;
    
}
static void remove_blocked(TCB* task)
{
    if(task == NULL){
        printf("\n### Error: trying to work with nullpointer. ###\n");
        return;
    }
    if(task->prev != NULL){
        task->prev->next = task->next;
    }
    else{
        blocked_head = task->next;
    }
    if(task->next != NULL){
        task->next->prev = task->prev;
    }
    
    task->next = NULL;
    task->prev = NULL;
}

static void tick(void)
{
    global_tick++;
    update_blocked();
}

static void update_blocked(void)
{
    while(blocked_head != NULL && blocked_head->wake_tick <= global_tick){
        TCB* task = blocked_head;
        remove_blocked(blocked_head);
        task->state = READY;
        insert_ready(task);
    }
}

void task_delay(int wait)
{
    if(current_tcb == NULL){ return; }
    current_tcb->wake_tick = global_tick + wait;
    insert_blocked(current_tcb);
}

void scheduler_run(void)
{
    while(1)
    {
        tick();
        printf("\n### global_tick count: %d\n\nWaiting list:\n", global_tick);
        if(blocked_head != NULL){
            TCB* foo = blocked_head;
            while(foo != NULL){
                printf("%s | wake: %d | remaining: %d\n", foo->op_name, foo->wake_tick, (foo->wake_tick - global_tick));
                foo = foo->next;
            }
        }printf("\n");
        printf("Ready list: \n");
        if(ready_head != NULL)
        {
            TCB* foo = ready_head->next;
            while(foo != NULL){
                printf("%s | priority: %d\n", foo->op_name, foo->priority);
                foo = foo->next;
            }printf("\n");
            current_tcb = ready_head;
            remove_ready(current_tcb);
            current_tcb->state = RUNNING;
            printf("RUNNING: %s | priority: %d\n", current_tcb->op_name, current_tcb->priority);

            current_tcb->task_function();
        }
        delay();
        delay();
    }
}
