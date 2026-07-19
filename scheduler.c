#include "scheduler.h"
#include "task.h"
#include <stdio.h>

TCB *taskList[5];
TCB *readyTasks[5] = {0};
int taskCount, readyCount = 0;
int tick = 0;
int size = sizeof(taskList)/sizeof(taskList[0]);

void scheduler_add_task(TCB *task)
{
    if(taskCount != 5)
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
    for(int i = 0; i < taskCount; i++){
            if(taskList[i]->state == BLOCKED){
                if(taskList[i]->wake_tick <= tick){
                    taskList[i]->state = READY;

                    int found = 0;
                    for(int j = 0; j < readyCount; j++){
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
        tick++;
        queue_update();
        if(readyTasks[0] != 0){
            readyTasks[0]->task_function(); delay();
            readyTasks[0]->wake_tick = tick + 7;
            readyTasks[0]->state = BLOCKED;
            readyCount--;
            for(int i = 0; i <readyCount; i++){
                readyTasks[i] = readyTasks[i + 1];
            } readyTasks[4] = 0;
        }
    }
}
