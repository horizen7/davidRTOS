#ifndef SYNCH_H
#define SYNCH_H

#include "tcb.h"

typedef struct Mutex{
    TCB* owner;
    TCB* wait_head;
    TCB* wait_tail;
} Mutex;

void mutex_init(Mutex*);
void mutex_lock(Mutex*);
void mutex_unlock(Mutex*);


typedef struct Semaphore{
    unsigned int count;
    TCB* wait_head;
    TCB* wait_tail;
} Semaphore;

void sema_init(Semaphore*);
void sema_post(Semaphore*);
void sema_wait(Semaphore*);
unsigned int sema_count(Semaphore*);

#endif
