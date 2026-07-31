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
void mutex_free(Mutex*);



#endif
