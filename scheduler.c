#include "scheduler.h"
#include "tcb.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

unsigned int global_tick;
static TCB* current_tcb;

static void idle_function(void){
    printf("waiting...\n");
}

void scheduler_init(void){
    global_tick = 0;
    current_tcb = NULL;

    task_create(idle_function, IDLE_PRIORITY, "idle_task");
}

TCB* get_current(void){
    return current_tcb; 
}

void set_current(TCB* setter){
    current_tcb = setter;
}

unsigned int get_global_tick(){
    return global_tick;
}

static void tick(void){
    global_tick++;
    queue_tick_blocked(global_tick);
}

void scheduler_run(void){
    while(1)
    {
        tick();
        current_tcb = pop_ready_queue();
        if(current_tcb != NULL)
        {
            current_tcb->state = RUNNING;
            printf("\nglobal_tick: %d\nRUNNING: %s | priority: %d\n", global_tick, current_tcb->op_name, current_tcb->priority);

            current_tcb->task_function();
            if(current_tcb->priority == IDLE_PRIORITY){
                
            }
            if(current_tcb != NULL){
                current_tcb->state = TERMINATED;
                delete_task(current_tcb);
            }
        }
        delay();
        delay();
    }
}
