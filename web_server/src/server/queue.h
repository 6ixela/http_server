#ifndef QUEUE
#define QUEUE

typedef struct queue
{
    int val;
    struct queue *next;
} queue;

queue* queue_push(queue* start, int val);
queue* queue_pop(queue* start, int *pval);
void queue_empty(queue** pstart);

#endif /* !SHARED_QUEUE */