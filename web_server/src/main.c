#include "config/config.h"
#include "server/server.h"
#include "server/action.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
    config conf = { 0 };
    if (create_config(&conf, argc, argv) == ERROR)
    {
        return 1;
    }

    action act = {NULL};
    get_action(&conf, &act);
    if (act.action != NULL)
    {
        act.action(&conf);
    }
    return 0;
}