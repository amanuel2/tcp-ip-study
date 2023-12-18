#ifndef CLI_H_
#define CLI_H_

#include "lib/syshead.h"
#include "apps/curl.h"


#define MAX_CMD_LENGTH 6

struct command {
    int args;
    int argc;
    char** argv;
    void* (* cmd_func) (void* arg);
    char* cmd_str;
};



struct command* parse_args(int argc, char** argv);

#endif