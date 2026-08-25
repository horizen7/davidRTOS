#include "synch.h"
#include "tcb.h"
#include <stdlib.h>
#include "scheduler.h"
#include <stdio.h>
#include "queue.h"
#include <stdint.h>

// MUTEX 

void mutex_init(Mutex* mutex){
    if(mutex == NULL){
        return;
    }
    *mutex = (Mutex){
            .owner = NULL,
            .wait_head = NULL,
        };
}

static void mutex_queue(Mutex* mutex, TCB* task){ //  essentially same as insert_blocked and insert_ready.
    if(mutex->wait_head == NULL){
        mutex->wait_head = task;

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
        printf("\n### ERROR: relocking mutex with same task. ###\n");
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
        };
}

static void sema_queue(Semaphore* sema, TCB* task){ //  essentially same as insert_blocked and insert_ready.
    if(sema == NULL || task == NULL){
        return;
    }
    if(sema->wait_head == NULL){
        sema->wait_head = task;

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

// EVENT FLAGS
// want to initialize, set, clear, read, and wait.

void event_init(EventGroup* event_group){
    if(event_group == NULL){
        return;
    }
    *event_group = (EventGroup){
        .flags = 0,
        .wait_head = NULL,
    };
}

static void event_single(TCB* task, EventGroup* event_group){
    if(task == NULL || event_group == NULL){
        return;
    }
    if(task->prev != NULL){
        task->prev->next = task->next;
    }
    else{
        event_group->wait_head = task->next;
    }
    if(task->next != NULL){
        task->next->prev = task->prev;
    }
    task->next = NULL;
    task->prev = NULL;
}

static void event_check(EventGroup* event_group, uint32_t event){
    /* go through event groups waitlist and check if any are waiting
       on the event passed through. if mode is wait_any wake task, remove from 
       blocked, if wait_all check all flags in event_group */
    if(event_group == NULL){
        return;
    }
    if(event_group->wait_head == NULL){
        return;
    }
    TCB* task = event_group->wait_head;
    while(task != NULL){
        TCB* temp = task->next;

        if(task->wait_flags & event){
            if(task->event_mode == WAIT_ANY){
                // take out of wait list then insert ready
                event_single(task, event_group);
                task->state = READY;
                insert_ready(task);
            }
            else if((event_group->flags & task->wait_flags) == task->wait_flags){ // checking for WAIT_ALL
                event_single(task, event_group);
                task->state = READY;
                insert_ready(task);
            }
        }
        task = temp;
    }
}

void event_set(EventGroup* event_group, uint32_t event){ // sets the respective event, then check waitlist to see if any were waiting on it, call event_check or something
    if(event_group == NULL){
        return;
    }
    event_group->flags |= event;
    event_check(event_group, event);
}

void event_clear(EventGroup* event_group, uint32_t event){
    if(event_group == NULL){
        return;
    }
    event_group->flags &= ~event;
}

void event_wait(EventGroup* event_group, uint32_t event){ // if the event hasnt happened, throw into blocked queue
    if(event_group == NULL){
        return;
    }
    if(event_group->flags & event){

    }
}

uint32_t event_get(EventGroup* event_group, uint32_t event){
    if(event_group == NULL){
        return;
    }
    return event_group->flags & event;
}