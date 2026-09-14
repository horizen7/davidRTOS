#include "synch.h"
#include "tcb.h"
#include <stdlib.h>
#include "scheduler.h"
#include <stdio.h>
#include "queue.h"
#include <stdint.h>

// generic queue and pop

static void synch_queue_insert(TCB** head, TCB* task){
    if(head == NULL || task == NULL){
        return;
    }
    if(*head == NULL){
        *head = task;

        task->prev = NULL;
        task->next = NULL;
        task->list_head = head;
        return;
    }
    if(task->priority > (*head)->priority){
        (*head)->prev = task;
        task->prev = NULL;
        task->next = *head;
        *head = task;
        task->list_head = head;
        return;
    }
    TCB* current = *head;
    TCB* previous = NULL;
    while(current != NULL && current->priority >= task->priority){
        previous = current;
        current = current->next;
    }
    previous->next = task;
    task->prev = previous;
    task->next = current;
    task->list_head = head;

    if(current != NULL){
        current->prev = task;
    }
}

static TCB* synch_pop(TCB** head){
    if(head == NULL || *head == NULL){
        return NULL;
    }
    TCB* task = *head;
    if(task->next != NULL){
        *head = task->next;
        (*head)->prev = NULL;
    }
    else{
        *head = NULL;
    }
    task->next = NULL;
    task->prev = NULL;
    task->list_head = NULL;

    return task;
}

// MUTEX 

static Mutex* mutex_list[MAX_MUTEX];
static uint16_t mutex_count = 0;

void mutex_init(Mutex* mutex){
    if(mutex == NULL){
        return;
    }
    if(mutex_count == MAX_MUTEX){
        printf("### Error: reached max mutex count. ###\n");
        return;
    }
    *mutex = (Mutex){
            .owner = NULL,
            .wait_head = NULL,
        };
    // now adding to master list
    mutex_list[mutex_count] = mutex;
    mutex_count++;
}

static void mutex_release(Mutex* mutex){ // internal function that actually reassigns ownership
    if(mutex == NULL){
        return;
    }
    if(mutex->wait_head == NULL){
        mutex->owner = NULL;
        return;
    }

    TCB* task = synch_pop(&mutex->wait_head);
    mutex->owner = task;
    if(task != NULL){
        task->state = READY;
        insert_ready(task);
    }
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
    else{ //  block task, throw into priority-based wait list.
        task->state = BLOCKED_MUTEX;
        synch_queue_insert(&mutex->wait_head, task);
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
        printf("### ERROR: attempting to unlock mutex it does not own. ###\n");
        return;
    }
    mutex_release(mutex);
}

void mutex_cleanup(TCB* task){ // look through master list of mutex check if it matches the owner of any release as necessary
    if(task == NULL){
        return;
    }
    for(uint16_t i = 0; i < mutex_count; i++){
        if(mutex_list[i]->owner == task){
            mutex_release(mutex_list[i]);
        }
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

void sema_post(Semaphore* sema){ // pop waitlist and insert ready, if no queue increment count.
    if(sema == NULL){
        return;
    }
    if(sema->wait_head != NULL){
        TCB* task = synch_pop(&sema->wait_head);
        if(task != NULL){
            task->state = READY;
            insert_ready(task);
        }
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
        synch_queue_insert(&sema->wait_head, task);
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

static void event_single(EventGroup* event_group, TCB* task){ // detach event from waitlist
    if(event_group == NULL || task == NULL){
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
    task->wait_flags = 0;
    task->list_head = NULL;
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
            if(task->wait_mode == WAIT_ANY){
                // take out of wait list then insert ready
                event_single(event_group, task);
                task->state = READY;
                insert_ready(task);
            }
            else if(task->wait_mode == WAIT_ALL){
                if((event_group->flags & task->wait_flags) == task->wait_flags){ // checking for WAIT_ALL
                    event_single(event_group, task);
                    task->state = READY;
                    insert_ready(task);
                }
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

void event_wait(EventGroup* event_group, uint32_t event, EventWaitMode mode){ // if the event hasnt happened, throw into blocked queue
    if(event_group == NULL){
        return;
    }
    TCB* task = get_current();
    if(task == NULL){
        return;
    }
    task->wait_flags = event;
    task->wait_mode = mode;

    if(mode == WAIT_ANY){
        if((event_group->flags & event) != 0){
            printf("Event conditions met.\n");
            return;
        }
    }
    else if((event_group->flags & event) == event){ // WAIT_ALL
        printf("Event conditions met.\n");
        return;
    }
    task->state = BLOCKED_EVENT;
    synch_queue_insert(&event_group->wait_head, task);
    set_current(NULL);
}

uint32_t event_get(EventGroup* event_group, uint32_t event){
    if(event_group == NULL){
        return 0;
    }
    return event_group->flags & event;
}