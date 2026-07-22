#include "scheduler.h"
#include "task.h"
#include "config.h"
#include <stdio.h>

void delay(void){ for(int i = 0; i < 1000000000; i++); }

void down(void) {
    printf("Ran task down. Priority: LOW\n");
    task_delay(2);
}

void hello(void) {
    printf("Ran task hello. Hello World!! Priority: HIGH\n");
    task_delay(5);
}

void count(void)
{
    printf("Ran task count. Priority: HIGH\n");
    task_delay(2);
}

void list(void)
{
    printf("Ran task list. Priority: MEDIUM\n");
    task_delay(7);
}

void point(void)
{
    printf("Ran task point. Priority: MEDIUM\n");
    task_delay(4);
}