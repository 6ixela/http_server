#define _POSIX_C_SOURCE 200809L

#include "config.h"
#include "../include/utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void parse_line(config *conf, char *line)
{
    char *saveptr;
    const char delimiter[] = " =\n";
    char *token = strtok_r(line, delimiter, &saveptr);
    char *data = strtok_r(NULL, delimiter, &saveptr);
    if (strcmp(token, "server_name") == 0)
    {
        strcpy(conf->server_name, data);
    }
    else if (strcmp(token, "port") == 0)
    {
        strcpy(conf->port, data);
    }
    else if (strcmp(token, "ip") == 0)
    {
        strcpy(conf->host, data);
    }
    else if (strcmp(token, "root_dir") == 0)
    {
        strcpy(conf->root_dir, data);
    }
    else
    {
        fprintf(stderr, "[WARNING][parse_line] %s not know", token);
    }
}

static int check_config(config *conf)
{
    if (strlen(conf->host) == 0)
    {
        strcpy(conf->host, "127.0.0.1");
    }
    else if (strlen(conf->port) == 0)
    {
        strcpy(conf->port, "2048");
    }
    else if (strlen(conf->root_dir) == 0)
    {
        perror("[ERROR][check_config] No root_dir given");
        return ERROR;
    }
    else if (strlen(conf->server_name) == 0)
    {
        perror("[ERROR][check_config] No root_dir given");
        return ERROR;
    }
    // check if the directory exist
    else if (strlen(conf->root_dir) != 0)
    {
        FILE *fd_directory = fopen(conf->root_dir, "r");
        if (fd_directory == NULL)
        {
            perror("[ERROR][check_config] root_dir does not exist");
            return ERROR;
        }
        fclose(fd_directory);
    }
    return SUCCESS;
}

static int parse_file(char *file_name, config *conf)
{
    FILE *fd = fopen(file_name, "r");
    if (fd == NULL)
    {
        perror("[ERROR][fopen] fopen return NULL.");
        return ERROR;
    }
    
    char *line;
    size_t len = 0;
    ssize_t nread;
    int err = SUCCESS;
    while ((nread = getline(&line, &len, fd)) != -1)
    {
        parse_line(conf, line);
    }
    
    check_config(conf);
    free(line);
    fclose(fd);
    return err;
}

static int get_action_enum(config *conf, char *action)
{
    if (strcmp(action, "start") == 0)
    {
        conf->action = START;
    }
    else if (strcmp(action, "stop") == 0)
    {
        conf->action = STOP;
    }
    else if (strcmp(action, "restart") == 0)
    {
        conf->action = RESTART;
    }
    else
    {
        return ERROR;
    }
    return SUCCESS;
}

static int parse_args(int argc, char *argv[], config *conf)
{
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--daemon") == 0 ||
            strcmp(argv[i], "-d") == 0)
        {
            conf->daemon = TRUE;
        }
        else if (strcmp(argv[i], "--action") == 0 ||
                 strcmp(argv[i], "-a") == 0)
        {
            i++;
            if (i >= argc)
            {
                perror("[ERROR][parse_args] --action requires an argument");
                return 1;
            }
            if (get_action_enum(conf, argv[i]) == ERROR)
            {
                perror("[ERROR][parse_args] --action (start | stop | restart)");
                return 1;
            }
        }
    }
    if (conf->action == NO_ACTION && conf->daemon == TRUE)
    {
        perror("[ERROR][parse_args] --action requires an argument for daemon mode");
        return 1;
    }
    
    return 0;
}

int create_config(config *conf, int argc, char *argv[])
{
    char file_name[64] = "config.txt";

    if (parse_args(argc, argv, conf) == 1)
    {
        return ERROR;
    }
    return parse_file(file_name, conf);
}