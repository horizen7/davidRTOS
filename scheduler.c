#include "scheduler.h"
#include "tcb.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include "synch.h"


unsigned int global_tick;
static TCB* current_tcb;

static void idle_function(void){
    printf("waiting...\n\n");
}

void scheduler_init(void){
    global_tick = 0;
    current_tcb = NULL;

    task_create(idle_function, IDLE_PRIORITY, "idle_task");
}

static void scheduler_print(void){
    printf("### global_tick: %d\n\n", global_tick);
    printf("RUNNING: %s, %d\n", current_tcb->op_name, current_tcb->priority);
    print_queues();
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
        // tick the global count, as long as the current tcb isnt null, run its task.
        // if task was idle task, throw it back into the ready list.
        // else check if current points to null, since delay or yield will throw tcb into respective list.
        tick();
        TCB* task = pop_ready_queue();
        current_tcb = task;
        if(task != NULL)
        {
            task->state = RUNNING;
            scheduler_print();
            task->task_function();
            
            if(task->priority == IDLE_PRIORITY){
                task->state = READY;
                insert_ready(task);
            }
            else if(task != NULL){
                task->state = TERMINATED;
                delete_task(task);
            }
        }
        delay();
        delay();
    }
}
