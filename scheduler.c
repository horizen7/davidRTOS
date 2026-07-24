#include "scheduler.h"
#include "config.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int global_tick = 0;

static TCB* current_tcb = NULL;



TCB* get_current(void){
    return current_tcb; 
}

void set_current(TCB* setter){
    current_tcb = setter;
}

static tick(void){
    global_tick++;
    queue_tick(global_tick);
}

void scheduler_run(void){
    while(1)
    {
        tick();
        printf("\n### global_tick count: %d\n\nWaiting list:\n", global_tick);
        if(blocked_head != NULL){
            TCB* foo = blocked_head;
            while(foo != NULL){
                printf("%s | wake: %d, remaining: %d\n", foo->op_name, foo->wake_tick, (foo->wake_tick - global_tick));
                foo = foo->next;
            }
        }printf("\n");
        printf("Ready list: \n");
        if(ready_head != NULL)
        {
            TCB* foo = ready_head->next;
            while(foo != NULL){
                printf("%s | priority: %d\n", foo->op_name, foo->priority);
                foo = foo->next;
            }printf("\n");
            current_tcb = ready_head;
            remove_ready(current_tcb);
            current_tcb->state = RUNNING;
            printf("RUNNING: %s | priority: %d\n", current_tcb->op_name, current_tcb->priority);

            current_tcb->task_function();

            if(current_tcb != NULL){
                current_tcb->state = TERMINATED;
                delete_task(current_tcb);
            }
        }
        delay();
        delay();
    }
}
