#include "scheduler.h"
#include "task.h"
#include <stdio.h>

void delay(void){ for(int i = 0; i < 1000000000; i++); }

void down(void) {
    printf("Counting down from 3, \n"); delay();
    for(int i = 2; i >= 0; i--){
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
    while(i < 2){
        printf("I am Davud Ro. ");
        i++;
        delay();
    } printf("\n");
}

void list(void)
{
    int arr[3] = {34, 634, 23};
    printf("Iterating through list.\n");
    for(int i = 0; i < 3; i++) {
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