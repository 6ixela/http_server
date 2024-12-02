#ifndef SHARED_QUEUE
#define SHARED_QUEUE

#include <semaphore.h>
#include "queue.h"

typedef struct shared_queue
{
    // Used to lock the push and pop operations.
    // (Similar to a mutex.)
    sem_t lock;
    sem_t size;

    queue* queue;
} shared_queue;

shared_queue* shared_queue_new();
void shared_queue_push(shared_queue* sq, int val);
int shared_queue_pop(shared_queue* sq);
void shared_queue_destroy(shared_queue* sq);

#endif /* !SHARED_QUEUE */