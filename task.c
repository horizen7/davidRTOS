#include "scheduler.h"
#include "task.h"
#include "config.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "synch.h"



TCB* master_list[MAX_TASKS];
unsigned int taskCount = 0;
Mutex test_mutex;
Semaphore test_sema;

// HELPER FUNCTIONS

void task_create(void (*function)(void), taskPriority priority, char name[]){
    TCB* new_tcb = malloc(sizeof(TCB));
    if(new_tcb == NULL){
        printf("\n### ERROR: unable to allocate memory. ###\n");
        return;
    }
    *new_tcb = (TCB){
        .task_function = function,
        .state = READY,
        .priority = priority,
        .wake_tick = 0,
        .next = NULL,
        .prev = NULL,
        .wait_flags = 0,
        .wait_mode = WAIT_ANY,
        .list_head = NULL
    };
    strcpy(new_tcb->op_name, name);
    if(taskCount < MAX_TASKS){
        master_list[taskCount] = new_tcb;
        taskCount++;
    }
    else{
        free(new_tcb);
        printf("\n### ERROR: max task count reached. ###\n");
        return;
    }
    insert_ready(new_tcb);
}

void delete_task(TCB* task){
    if(task == NULL){
        return;
    }
    for(unsigned int i = 0; i < taskCount; i++){
        if(master_list[i] == task){
            mutex_cleanup(task); // release mutexes owned by task
            remove_from_list(task);
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
    task->wake_tick = get_global_tick() + wait;
    task->state = BLOCKED;
    insert_blocked(task);
    set_current(NULL);
}

void task_yield(void){
    if(get_current() == NULL){
        return;
    }
    TCB* task = get_current();
    task->state = READY;
    insert_ready(task);
    set_current(NULL);
}

void delay(void){ for(int i = 0; i < 1000000000; i++); }

// CUSTOM TASKS

void high_task(void){
    printf("high: hello\n\n");
}
void delay_task(void){
    printf("delay: sleeping for 3 ticks\n\n");
    task_delay(4);
}
void yield_task(void){
    printf("yield: yielding\n\n");
    //task_yield();
}
void mutex_locker(void){
    printf("mutex_locker: locking mutex\n\n");
    mutex_lock(&test_mutex);
}
void mutex_unlocker(void){
    printf("mutex_unlocker: unlocking\n\n");
    mutex_unlock(&test_mutex);
}
void mutex_waiter(void){
    printf("mutex_waiter: trying mutex\n\n");
    mutex_lock(&test_mutex);

    //printf("acquired mutex\n\n");
    //mutex_unlock(&test_mutex);
}
void sema_waiter(void){
    sema_wait(&test_sema);
    printf("sema_waiter: waiting for token, count = %d\n\n", sema_count(&test_sema));
}
void sema_poster(void){
    sema_post(&test_sema);
    printf("sema_poster: posting semaphore, count = %d\n\n", sema_count(&test_sema));
}
void quick_task(void){
    printf("quick: done, terminating\n\n");
}