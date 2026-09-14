#include "queue.h"
#include "tcb.h"
#include "scheduler.h"
#include "synch.h"
#include "task.h"
#include <stdlib.h>
#include <stdio.h>

static TCB* ready_head = NULL;
static TCB* blocked_head = NULL;



void insert_ready(TCB* task){
    // checking if theres nothing, then if it has higher priority than the head.
    // then just going through until it finds something less than and inserting itself in the previous position.
    if(task == NULL){
        return;
    }
    if(ready_head == NULL){
        ready_head = task;
        task->prev = NULL;
        task->next = NULL;
        task->list_head = &ready_head;
        return;
    }
    if(task->priority > ready_head->priority){
        ready_head->prev = task;
        task->next = ready_head;
        task->prev = NULL;
        ready_head = task;
        task->list_head = &ready_head;
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
    task->list_head = &ready_head;

    if(current != NULL){
        current->prev = task;
    }
}

void insert_blocked(TCB* task){ // same as above ^^.
    if(task == NULL){
        return;
    }
    if(blocked_head == NULL){
        blocked_head = task;
        task->prev = NULL;
        task->next = NULL;
        task->list_head = &blocked_head;
        return;
    }
    if(task->wake_tick < blocked_head->wake_tick){
        blocked_head->prev = task;
        task->next = blocked_head;
        task->prev = NULL;
        blocked_head = task;
        task->list_head = &blocked_head;
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
    task->list_head = &blocked_head;

    if(current != NULL){
        current->prev = task;
    }
}

void remove_from_list(TCB* task){ // removes tcb from any list.
    if(task == NULL || task->list_head == NULL){
        printf("\n### ERROR: trying to work with nullpointer or not in list. ###\n");
        return;
    }
    if(task->prev != NULL){
        task->prev->next = task->next;
    }
    else{
        *(task->list_head) = task->next;
    }
    if(task->next != NULL){
        task->next->prev = task->prev;
    }
    
    task->next = NULL;
    task->prev = NULL;
    task->list_head = NULL;
}

void queue_tick_blocked(unsigned int tick){
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

void print_queues(void){ // terminal print helper function
    TCB* temp = ready_head;
    while(temp != NULL){
        printf("READY: %s, %d\n", temp->op_name, temp->priority);
        temp = temp->next;
    }
    temp = blocked_head;
    while(temp != NULL){
        printf("BLOCKED: %s, %d, ticks till wake: %d\n", temp->op_name, temp->priority, (temp->wake_tick - get_global_tick()));
        temp = temp->next;
    }
    
    // for mutex and semaphore blocked tasks.
    int i;
    for(i = 0; i < taskCount; i++){
        temp = master_list[i];
        if(temp->state == BLOCKED_MUTEX){
            printf("BLOCKED_MUTEX: %s, %d\n", temp->op_name, temp->priority);
        }
    }
    for(i = 0; i < taskCount; i++){
        temp = master_list[i];
        if(temp->state == BLOCKED_SEMAPHORE){
            printf("BLOCKED_SEMAPHORE: %s, %d\n", temp->op_name, temp->priority);
        }
    }
    printf("\n");
}