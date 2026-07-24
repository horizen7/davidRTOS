#include "queue.h"
#include "scheduler.h"
#include <stdlib.h>
#include <stdio.h>

static TCB* ready_head = NULL;
static TCB* blocked_head = NULL;



void insert_ready(TCB* task){
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

void insert_blocked(TCB* task){
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
void remove_ready(TCB* task){
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
void remove_blocked(TCB* task){
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

void queue_tick(unsigned int tick){
    while(blocked_head != NULL && blocked_head->wake_tick <= tick){
        TCB* task = blocked_head;
        remove_blocked(blocked_head);

        task->state = READY;
        insert_ready(task);
    }
}

TCB* pop_ready_queue(void){
    if(ready_head == NULL){
        return NULL;
    }
    TCB* task = ready_head;
    remove_ready(ready_head);

    return task;
}

TCB* pop_blocked_queue(void){
    if(blocked_head == NULL){
        return NULL;
    }
    TCB* task = blocked_head;
    remove_blocked(blocked_head);

    return task;
}
