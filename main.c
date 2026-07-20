#include <stdio.h>
#include "task.h"
#include "scheduler.h"
#include "config.h"

TCB downTCB = {
    .task_function = &down,
    .priority = 1,
    .state = READY,
    .interval = 7
};

TCB helloTCB = {
    .task_function = &hello,
    .priority = 3,
    .state = READY,
    .interval = 8
};

TCB countTCB = {
    .task_function = &count,
    .priority = 3,
    .state = READY,
    .interval = 2
};

TCB listTCB = {
    .task_function = &list,
    .priority = 2,
    .state = READY,
    .interval = 3
};

TCB pointTCB = {
    .task_function = &point,
    .priority = 1,
    .state = READY,
    .interval = 5
};

int main(void)
{
    scheduler_add_task(&downTCB);
    scheduler_add_task(&helloTCB);
    scheduler_add_task(&countTCB);
    scheduler_add_task(&listTCB);
    scheduler_add_task(&pointTCB);

    scheduler_run();
    
    return 0;
}