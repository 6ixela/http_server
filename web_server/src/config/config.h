#ifndef CONFIG_H
#define CONFIG_H

#define ERROR 1
#define SUCCESS 0

typedef struct config
{
    char root_dir[64];
    char server_name[32];
    char host[32];
    char port[8];
} config;

int create_config(config *conf, int argc, char *argv[]);
#endif /* CONFIG_H */