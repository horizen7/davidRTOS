#include "synch.h"
#include "tcb.h"
#include "stdlib.h"
#include "scheduler.h"

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

void mutex_lock(Mutex* mutex){
    if(mutex == NULL){
        return;
    }
    else if(mutex->owner == NULL){
        mutex->owner = get_current();
        return;
    }
    
}

void mutex_free(Mutex* mutex){
    //
}