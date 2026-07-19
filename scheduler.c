#include "scheduler.h"
#include "task.h"
#include <stdio.h>
#define size 5

TCB *taskList[size];
TCB *readyTasks[size] = {0};
unsigned int taskCount, readyCount, tick = 0;

void scheduler_add_task(TCB *task)
{
    if(taskCount < size)
    {
        taskList[taskCount] = task;
        taskCount++;
        if(task->state == READY){
            readyTasks[readyCount] = task;
            readyCount++;
        }
    }
    else{ printf("Too much going on in my head.."); }
}

void queue_update(void)
{
    for(unsigned int i = 0; i < taskCount; i++){
            if(taskList[i]->state == BLOCKED){
                if(taskList[i]->wake_tick <= tick){
                    taskList[i]->state = READY;

                    int found = 0;
                    for(unsigned int j = 0; j < readyCount; j++){
                        if(readyTasks[j] == taskList[i]){
                            found = 1;
                            break;
                        }
                    }
                    if(!found && readyCount < 5){
                        readyTasks[readyCount] = taskList[i];
                        readyCount++;
                    }
                }
            }
        }
}

void scheduler_run(void)
{
    while(1)
    {
        if(readyCount > 0)
        {
            unsigned int i;
            readyTasks[0]->state = RUNNING;
            for(i = 0; i < taskCount; i++){
                printf("Task %d wait time: %d, state: %d.\n", (i + 1), (taskList[i]->wake_tick - tick), taskList[i]->state);
            }
            readyTasks[0]->task_function();
            readyTasks[0]->wake_tick = tick + readyTasks[0]->interval;
            readyTasks[0]->state = BLOCKED;
            readyCount--;
            for(i = 0; i <= readyCount; i++){
                readyTasks[i] = readyTasks[i + 1];
            } readyTasks[readyCount] = NULL;
        }
        else{
            printf("\nwaiting...\n");
        }
        delay();
        tick++;
        queue_update();
    }
}
