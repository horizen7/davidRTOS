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

## Tick Members/Blocked Queue Algorithm

Problem:
Not sure what tick variables I should hold within the TCBs. If I want to order the blocked queue in ascending wake tick order, would be nice to just put that in.

Decision:
Will have a wake tick member in the TCB struct that is initialized using the current global tick count.

Implementation:
Having a function that takes in the duration of delay as a parameter and assigns the TCBs wake tick according to that and the current tick count. I think making the function also return the wake tick value in order to iterate through the blocked queue to compare could work. 

Tradeoff:

## 

Problem:

Decision:

Implementation:

Tradeoff:

## 

Problem:

Decision:

Implementation:

Tradeoff:
