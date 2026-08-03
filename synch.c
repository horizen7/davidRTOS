#include "synch.h"
#include "tcb.h"
#include <stdlib.h>
#include "scheduler.h"
#include <stdio.h>
#include "queue.h"


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
        task = READY;
        insert_ready(task);
    }
}