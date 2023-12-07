#include "cli.h"

extern void* curl(void *arg);
extern int running;

// 0 -> core networking
// 1 -> integrated apps

void* usage(void* arg) {
    struct command *cmd = arg;
    printf("Usage: sudo %s [command ARGS..]\n\n", cmd->argv[0]);
    printf("Commands:\n");
    printf("  curl HOST - act like curl, HOST as the target.\n");
    printf("\n");
    printf("Elevated privileges are needed because of tuntap devices.\n");
    printf("See https://www.kernel.org/doc/Documentation/networking/tuntap.txt\n");
    
    running = 0;
    return 0;
};

void* noop(void* arg) {
    return 0;
}


static struct command cmds[] = {
    { 0, 0, NULL, usage, "help" },
    { 1, 0, NULL, curl, "curl" },
    { 0, 0, NULL, noop, "noop"},
    { 0, 0, NULL, NULL, NULL }
};



struct command* parse_args(int argc, char** argv) {
    struct command *cmd = &cmds[2];
    if(argc == 1)  return cmd;
    
    
    for(cmd = &cmds[0]; cmd->cmd_func; cmd++) {
        printf("INITALIZING Command %s\n", cmd->cmd_str);
        if(strncmp(argv[1], cmd->cmd_str, 6) == 0) {
            cmd->argc = argc - 2; // dont include filename+command_name
            cmd->argv = &argv[2];
            
            if(cmd->argc != cmd->args) break;
            return cmd;
        }
    }
    
    return &cmds[0];
}