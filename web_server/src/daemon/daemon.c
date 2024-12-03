#include "daemon.h"

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static int *stop = NULL;

static void handler(int signum)
{
    switch (signum)
    {
    case SIGINT:
        printf("Got SIGINT\n");
        *stop = 1;
        break;
    default:
        break;
    }
}

static int write_file(char *pid_file, int id)
{
    FILE *fd = fopen(pid_file, "w+");

    if (fd == NULL)
    {
        return -1;
    }
    fprintf(fd, "%i\n", id);

    fclose(fd);
    return 0;
}

int create_daemon(int *stop_while)
{
    // TODO: write in log file if daemon is running
    stop = stop_while;
    int id = fork();
    if (id == -1)
    {
        perror("[ERROR][fork] Create daemon");
        return -1;
    }

    struct sigaction sa;
    if (id > 0)
    {
        exit(0);
    }

    if (write_file("pid_file.txt", getpid()) == -1)
    {
        return -1;
    }

    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigemptyset(&sa.sa_mask) < 0)
    {
        perror("[ERROR][sigaction] Sigemptyset");
        return -1;
    }
    if (sigaction(SIGINT, &sa, NULL) < 0)
    {
        perror("[ERROR][sigaction] problem catch signal");
        return -1;
    }
    return 0;
}