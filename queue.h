#ifndef QUEUE_H
#define QUEUE_H

#include "tcb.h"

void insert_ready(TCB* task);
void insert_blocked(TCB* task);
void remove_from_list(TCB* task);
void queue_tick_blocked(unsigned int);
TCB* pop_ready_queue(void);
TCB* pop_blocked_queue(void);
void print_queues(void);

#endif