#include <stdio.h>
#include "task.h"
#include "scheduler.h"
#include "config.h"

int main(void)
{
    create_tcb(down, READY, LOW, "down");
    create_tcb(hello, READY, HIGH, "hello");
    create_tcb(count, READY, HIGH, "count");
    create_tcb(list, READY, MEDIUM, "list");
    create_tcb(point, READY, MEDIUM, "point");

    scheduler_run();
    
    return 0;
}