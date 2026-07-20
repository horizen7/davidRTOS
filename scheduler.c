#include "scheduler.h"
#include "task.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>

unsigned int taskCount, readyCount, tick = 0;

TCB* master_list[MAX_TASKS];
static int task_count = 0;
static TCB* ready_head = NULL;
static TCB* blocked_head = NULL;


void create_tcb(void (*function)(void), taskState state, taskPriority priority, unsigned int interval)
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
        .interval = interval,
    };
    if(task_count < MAX_TASKS){
        master_list[task_count] = new_tcb;
        taskCount++;
    }
    task_insert(new_tcb);
}

static void task_insert(TCB* task)
{
    switch(task->state){
        case READY:
        ready_insert(task);
        break;
        case BLOCKED:
        blocked_insert(task);
        break;
        case RUNNING:
        printf("\n### Error: tried inserting running task. ###\n");
        break;
    }
}

static void task_remove(TCB* task)
{
    switch(task->state){
        case READY:
        ready_remove(task);
        break;
        case BLOCKED:
        blocked_remove(task);
        break;
        case RUNNING:
        printf("\n### Error: tried removing running task. ###\n");
        break;
    }
}

static void ready_insert(TCB* task)
{
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

static void blocked_insert(TCB* task);
static void ready_remove(TCB* task);
static void blocked_remove(TCB* task);

static void queue_update(void)
{
    for(unsigned int i = 0; i < taskCount; i++){
            if(taskList[i]->state == BLOCKED){
                if(taskList[i]->wake_tick <= tick){
                    taskList[i]->state = READY;

                    int found = 0;
                    for(unsigned int j = 0; j < readyCount; j++){
                        if(readyTasks[j] == taskList[i]){
                            found = 1;
                            break;
                        }
                    }
                    if(!found && readyCount < 5){
                        readyTasks[readyCount] = taskList[i];
                        readyCount++;
                    }
                }
            }
        }
}

void scheduler_run(void)
{
    while(1)
    {
        if(readyCount > 0)
        {
            unsigned int i;
            readyTasks[0]->state = RUNNING;
            for(i = 0; i < taskCount; i++){
                printf("Task %d wait time: %d, state: %d.\n", (i + 1), (taskList[i]->wake_tick - tick), taskList[i]->state);
            }
            readyTasks[0]->task_function();
            readyTasks[0]->wake_tick = tick + readyTasks[0]->interval;
            readyTasks[0]->state = BLOCKED;
            readyCount--;
            for(i = 0; i <= readyCount; i++){
                readyTasks[i] = readyTasks[i + 1];
            } readyTasks[readyCount] = NULL;
        }
        else{
            printf("\nwaiting...\n");
        }
        delay();
        tick++;
        queue_update();
    }
}
