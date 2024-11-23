#include <err.h>
#include <stdlib.h>
#include "queue.h"

queue* queue_push(queue* start, int val)
{
    queue *q = malloc(sizeof(queue));
    if(q == NULL)
        errx(1, "not enougth memory\n");
    q->val = val;

    if (start == NULL)
        q->next = q;
    else
    {
        q->next = start->next;
        start->next = q;
    }
    return q;
}

queue* queue_pop(queue* start, int *pval)
{
    if (start != NULL)
    {
        queue *q = start->next;
        *pval = q->val;
        if (q->next == q)
            start = NULL;
        else
            start->next = q->next;
        free(q);
    }
    return start;
}

void queue_empty(queue** pstart)
{
    int pval;
    queue* q = queue_pop(*pstart, &pval);
    while (q != NULL)
        q = queue_pop(*pstart, &pval); 
}
