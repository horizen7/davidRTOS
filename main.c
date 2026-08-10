#include <stdio.h>
#include "task.h"
#include "scheduler.h"
#include "tcb.h"
#include "config.h"
#include "synch.h"

int main(void){

    mutex_init(&test_mutex);
    sema_init(&test_sema, 1);

    task_create(high_task, HIGH, "high");
    task_create(delay_task, LOW, "delay");
    task_create(yield_task, LOW, "yield");
    task_create(mutex_owner, MEDIUM, "mutex_owner");
    task_create(mutex_waiter, MEDIUM, "mutex_waiter");
    task_create(sema_waiter, LOW, "sema_wait");
    task_create(sema_poster, LOW, "sema_post");
    task_create(quick_task, MEDIUM, "quick");

    scheduler_init();
    scheduler_run();
    
    return 0;
}