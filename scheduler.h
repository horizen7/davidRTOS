#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "tcb.h"

void scheduler_run(void);
TCB* get_current(void);
void set_current(TCB* setter);
unsigned int get_global_tick();
void scheduler_init(void);


#endif