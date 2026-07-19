#include "scheduler.h"
#include "task.h"
#include <stdio.h>

void delay(void){ for(int i = 0; i < 100000000; i++); }

void down(void) {
    printf("Counting down from 11, \n"); delay();
    for(int i = 10; i >= 0; i--){
        printf("%d,\n", i);
        delay();
    }
}

void hello(void) {
    printf("Hello World!!\n");
}

void count(void)
{
    int i = 0;
    while(i < 4){
        printf("I am Davud Ro. ");
        i++;
        delay();
    } printf("\n");
}

void list(void)
{
    int arr[10] = {34, 634, 23, 435, 87, 9, 3234, 87 ,67, 1};
    printf("Iterating through list.\n");
    for(int i = 0; i < 10; i++) {
        printf("Index: %d | Element: %d\n", i, arr[i]);
        delay();
    }
}

void point(void)
{
    int x = 30;
    int *ptr = &x;

    printf("%d, %p, %d\n", x, (void*)ptr, *ptr);
}