#include <err.h>
#include <stdlib.h>
#include "shared_queue.h"


shared_queue* shared_queue_new()
{
    shared_queue * sq = malloc(sizeof(shared_queue));
    if(sq == NULL)
        errx(1, "erreur not enough\n");

    int err = sem_init(&(sq->lock), 0, 1);
    if(err == -1)
        errx(1, "sem init lock probleme\n");

    err = sem_init(&(sq->size), 0, 0);
    if(err == -1)
        errx(1, "sem init size probleme\n");
        
    sq->queue = NULL;
    return sq;
}

void shared_queue_push(shared_queue* sq, int val)
{
    if(sem_wait(&(sq->lock))==-1)
        errx(1, "erreur semaphore\n");

    sq->queue = queue_push(sq->queue, val);

    if(sem_post(&(sq->lock))==-1)
        errx(1, "erreur semaphore\n");
    if(sem_post(&(sq->size))==-1)
        errx(1, "erreur semaphore\n");
}

int shared_queue_pop(shared_queue* sq)
{
    int res = 0;
    if(sem_wait(&(sq->size))==-1)
        errx(1, "erreur semaphore\n");
    if(sem_wait(&(sq->lock))==-1)
        errx(1, "erreur semaphore\n");

    sq->queue = queue_pop(sq->queue, &res);

    if(sem_post(&(sq->lock))==-1)
        errx(1, "erreur semaphore\n");
    return res;
}

void shared_queue_destroy(shared_queue* sq)
{
    queue_empty(&(sq->queue));
    if(sem_destroy(&(sq->lock))==-1)
        errx(1, "erreur semaphore\n");
    if(sem_destroy(&(sq->size))==-1)
        errx(1, "erreur semaphore\n");
    //free(sq->queue);
    free(sq);
}