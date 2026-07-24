#include "scheduler.h"
#include "task.h"
#include "queue.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>

TCB* master_list[MAX_TASKS];
unsigned int taskCount = 0;

/*** HELPER FUNCTIONS ***/

void task_create(void (*function)(void), taskPriority priority, char name[]){
    TCB* new_tcb = malloc(sizeof(TCB));
    if(new_tcb == NULL){
        printf("\n### Error: unable to allocate memory. ###\n");
        return;
    }
    *new_tcb = (TCB){
        .task_function = function,
        .state = READY,
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
        free(new_tcb);
        printf("\n### Error: max task count reached. ###\n");
        return;
    }
    insert_ready(new_tcb);
}

void delete_task(TCB* task){
    for(unsigned int i = 0; i < taskCount; i++){
        if(master_list[i] == task){
            switch(task->state){
                case READY:
                    remove_ready(task);
                    break;
                case BLOCKED:
                    remove_blocked(task);
                    break;
                case RUNNING:
                    return;
                case TERMINATED:
                    return;
            }

            for(unsigned int j = i; j < taskCount - 1; j++){
                master_list[j] = master_list[j + 1];
            }
            if(task == get_current()){
                set_current(NULL);
            }
            free(task);
            taskCount--;
            master_list[taskCount] = NULL;
            
            
            return;
        }
    }
}

void task_delay(int wait){
    if(get_current() == NULL){
        return;
    }
    TCB* task = get_current();
    task->wake_tick = global_tick + wait;
    task->state = BLOCKED;
    insert_blocked(task);
    set_current(NULL);
}

void yield_task(void){
    if(get_current == NULL){
        return;
    }
    TCB* task = get_current();
    task->state = READY;
    insert_ready(task);
    set_current(NULL);
}

void delay(void){ for(int i = 0; i < 1000000000; i++); }

/*** TASKS ***/

void down(void){ task_delay(2); }

void hello(void){ printf("Hello World!!"); }

void count(void){ task_delay(2); }

void list(void){ printf("long day."); }

void point(void){ task_delay(4); }