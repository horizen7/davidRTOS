#include <stdio.h>
#include "task.h"
#include "scheduler.h"
#include "config.h"

int main(void)
{
    task_create(down, READY, LOW, "down");
    task_create(hello, READY, HIGH, "hello");
    task_create(count, READY, HIGH, "count");
    task_create(list, READY, MEDIUM, "list");
    task_create(point, READY, MEDIUM, "point");

    scheduler_run();
    
    return 0;
}