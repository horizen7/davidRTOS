#ifndef SYNCH_H
#define SYNCH_H

#include "tcb.h"
#include <stdint.h>
#include "config.h"

typedef struct Mutex{
    TCB* owner;
    TCB* wait_head;
} Mutex;

void mutex_init(Mutex*);
void mutex_lock(Mutex*);
void mutex_unlock(Mutex*);
void mutex_cleanup(TCB*);

typedef struct Semaphore{
    unsigned int count;
    TCB* wait_head;
} Semaphore;

void sema_init(Semaphore*, unsigned int);
void sema_post(Semaphore*);
void sema_wait(Semaphore*);
unsigned int sema_count(Semaphore*);

typedef struct EventGroup{
    uint32_t flags;
    TCB* wait_head;
} EventGroup;

typedef enum {
    WAIT_ALL,
    WAIT_ANY
} EventWaitMode;

void event_init(EventGroup*);
void event_set(EventGroup*, uint32_t);
void event_clear(EventGroup*, uint32_t);
void event_wait(EventGroup*, uint32_t, EventWaitMode);
uint32_t event_get(EventGroup*, uint32_t);

#endif
