
# Phase 1:

We implement the queue using a doubly linked list.
A queue structure is designed.Used to record the head and tail of the queue, and the length of the queue.
A queue node structure is designed.It's used to store data.
The queue node also has two Pointers, one to the previous node and one to the next node.

# Phase 2:

We're using a queue in phase one to store threads.
A structure is designed to record thread IDs, as well as context
The uthread_start function initializes the thread library.
Uthread_create, we create a new thread and put it in the wait queue.
The uthread_yield function, which removes the oldest thread from the head of the queue, implements the context switch.
The uthread_stop function, we recycle the thread library.
The uthread_join function, we just implement a simple loop.
Uthread_exit function, we switch the context and replace the terminated thread.

