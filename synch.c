#include "synch.h"
#include "tcb.h"
#include <stdlib.h>
#include "scheduler.h"
#include <stdio.h>
#include "queue.h"


// MUTEX 

void mutex_init(Mutex* mutex){
    if(mutex == NULL){
        return;
    }
    *mutex = (Mutex){
            .owner = NULL,
            .wait_head = NULL,
            .wait_tail = NULL
        };
}

static void mutex_queue(Mutex* mutex, TCB* task){ //  essentially same as insert_blocked and insert_ready.
    if(mutex->wait_head == NULL){
        mutex->wait_head = task;
        mutex->wait_tail = task;

        task->prev = NULL;
        task->next = NULL;
        return;
    }
    if(task->priority > mutex->wait_head->priority){
        mutex->wait_head->prev = task;
        task->prev = NULL;
        task->next = mutex->wait_head;
        mutex->wait_head = task;
        return;
    }
    TCB* current = mutex->wait_head;
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
    else{
        mutex->wait_tail = task;
    }
}

static TCB* mutex_pop(Mutex* mutex){
    if(mutex->wait_head == NULL){
        return NULL;
    }
    TCB* task = mutex->wait_head;
    if(task->next != NULL){
        mutex->wait_head = task->next;
        mutex->wait_head->prev = NULL;
    }
    else{
        mutex->wait_head = NULL;
        mutex->wait_tail = NULL;
    }
    task->next = NULL;
    task->prev = NULL;

    return task;
}

void mutex_lock(Mutex* mutex){
    TCB* task = get_current();

    if(mutex == NULL || task == NULL){
        return;
    }
    else if(mutex->owner == NULL){
        mutex->owner = task;
        return;
    }
    else if(mutex->owner == task){
        printf("\n### Error: relocking mutex with same task. ###\n");
        return;
    }
    else{ //  block task, throw into the back of waiting list.
        task->state = BLOCKED_MUTEX;
        mutex_queue(mutex, task);
        set_current(NULL);
    }
    
}

void mutex_unlock(Mutex* mutex){ // free ownership, pop waitlist and assign new king.
    if(mutex == NULL){
        return;
    }
    if(mutex->owner == NULL){
        return;
    }
    if(mutex->owner != get_current()){
        return;
    }
    if(mutex->wait_head == NULL){
        mutex->owner = NULL;
        return;
    }

    TCB* task = mutex_pop(mutex);
    mutex->owner = task;
    if(task != NULL){
        task->state = READY;
        insert_ready(task);
    }
}

// SEMAPHORE

void sema_init(Semaphore* sema, unsigned int count){
    if(sema == NULL){
        return;
    }
    *sema = (Semaphore){
            .count = count,
            .wait_head = NULL,
            .wait_tail = NULL
        };
}

static void sema_queue(Semaphore* sema, TCB* task){ //  essentially same as insert_blocked and insert_ready.
    if(sema == NULL || task == NULL){
        return;
    }
    if(sema->wait_head == NULL){
        sema->wait_head = task;
        sema->wait_tail = task;

        task->prev = NULL;
        task->next = NULL;
        return;
    }
    if(task->priority > sema->wait_head->priority){
        sema->wait_head->prev = task;
        task->prev = NULL;
        task->next = sema->wait_head;
        sema->wait_head = task;
        return;
    }
    TCB* current = sema->wait_head;
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
    else{
        sema->wait_tail = task;
    }
}

static TCB* sema_pop(Semaphore* sema){
    if(sema->wait_head == NULL){
        return NULL;
    }
    TCB* task = sema->wait_head;
    if(task->next != NULL){
        sema->wait_head = task->next;
        sema->wait_head->prev = NULL;
    }
    else{
        sema->wait_head = NULL;
        sema->wait_tail = NULL;
    }
    task->next = NULL;
    task->prev = NULL;

    return task;
}

void sema_post(Semaphore* sema){ // pop waitlist and insert ready, if no queue increment count.
    if(sema == NULL){
        return;
    }
    if(sema->wait_head != NULL){
        TCB* task = sema_pop(sema);
        
        task->state = READY;
        insert_ready(task);
    }
    else{
        sema->count++;
    }
}

void sema_wait(Semaphore* sema){ // task asking for token, behave based on count.
    if(sema == NULL){
        return;
    }
    if(sema->count > 0){
        sema->count--;
    }
    else{
        TCB* task = get_current();
        if(task == NULL){
            return;
        }

        // at this point the scheduler should move onto the next task.
        task->state = BLOCKED_SEMAPHORE;
        sema_queue(sema, task);
        set_current(NULL);
    }
}

unsigned int sema_count(Semaphore* sema){
    if(sema == NULL){
        return 0;
    }
    return sema->count;
}