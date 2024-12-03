#include "action.h"

#include "../daemon/daemon.h"
#include "server.h"

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
static int recup_pid(char *file_name)
{
    int res = 0;
    int fd = open(file_name, O_RDONLY);
    char buf[16];
    read(fd, buf, 16);
    res = atoi(buf);
    close(fd);
    return res;
}

int get_action(config *conf, action *action)
{
    if (conf->daemon)
    {
        int pid = recup_pid("pid_file.txt");
        if (pid != 0)
        {
            if (conf->action == START)
            {
                perror("[ERROR][start] server already started");
                return ERROR;
            }
            else if (conf->action == STOP)
            {
                action->action = stop_server;
                conf->stop = pid;
                remove("pid_file.txt");
                return 0;
            }
        }
        else
        {
            if (conf->action == START)
            {
                action->action = start_server;
            }
            else
            {
                perror("[ERROR] server not started");
                return ERROR;
            }
        }
        create_daemon(&conf->stop);
    }
    else
    {
        action->action = start_server;
    }
    return 0;
}