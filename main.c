#include <stdio.h>
#include "task.h"
#include "scheduler.h"
#include "tcb.h"
#include "config.h"
#include "synch.h"

int main(void){

    mutex_init(&test_mutex);

    task_create(high_task, HIGH, "high");
    task_create(delay_task, MEDIUM, "delay");
    task_create(yield_task, MEDIUM, "yield");
    task_create(mutex_owner, LOW, "owner");
    task_create(mutex_waiter, LOW, "waiter");

    scheduler_init();
    scheduler_run();
    
    return 0;
}