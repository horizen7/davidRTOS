#include <stdio.h>
#include "task.h"
#include "scheduler.h"
#include "tcb.h"
#include "config.h"

int main(void){

    task_create(down, LOW, "down");
    task_create(hello, HIGH, "hello");
    task_create(count, HIGH, "count");
    task_create(list, MEDIUM, "list");
    task_create(point, MEDIUM, "point");

    scheduler_run();
    
    return 0;
}