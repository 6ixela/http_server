#include "config/config.h"
#include "server/server.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
    config conf = { 0 };
    if (create_config(&conf, argc, argv) == ERROR)
    {
        return 1;
    }

    start_server(&conf);
    return 0;
}