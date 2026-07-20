# Design Decisions #


## Scheduler Algorithm

Problem:
Looking though linked list to find the next highest priority task takes a bit of time. Messy code in scheduler_run() as well.

Decision:
Priority-based queue insertion.

Implementation:
When the TCBs are created, a function that calls another function based on the new TCBs state is called. The function that is called in respect to the state then looks through a linked list and inserts the TCB node when a node with a lower priority is found, or it reaches the end. 

Tradeoff:
Had to use doubly linked lists and make a bunch of helper functions.

##
