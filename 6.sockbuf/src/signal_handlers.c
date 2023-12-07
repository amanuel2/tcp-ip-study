#include "signal_handlers.h"

extern int running;

void stop_stack_handler(int signo) {
    printf("Stopping....\n");
    running = 0;
}


void* _signal(int signo, sighandler_t handler) {
    char s[200];
    struct sigaction sa = {
        .sa_handler = handler, 
        .sa_flags = SA_RESTART,
        .sa_mask = 0
    };
    
    if(sigaction(signo, &sa, NULL) < 0) { 
        return SIG_ERR;
    }
    
    
    return 0;
}

/*inline*/ void init_signals() {
    _signal(SIGINT, stop_stack_handler); // CTRL^C
}
