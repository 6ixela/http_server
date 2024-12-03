#ifndef ACTION_H
#define ACTION_H

#include "../config/config.h"

typedef struct action
{
    void (*action)(config *conf) ;
} action;


int get_action(config *conf, action *action);

#endif /* ACTION_H */