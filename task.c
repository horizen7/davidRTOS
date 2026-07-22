#include "scheduler.h"
#include "task.h"
#include "config.h"
#include <stdio.h>

void delay(void){ for(int i = 0; i < 1000000000; i++); }

void down(void){ task_delay(2); }

void hello(void){ task_delay(5); }

void count(void){ task_delay(2); }

void list(void){ task_delay(7); }

void point(void){ task_delay(4); }