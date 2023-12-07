#include "syshead.h"
#include "basic.h"
#include "tuntap_if.h"
#include "utils.h"
#include "ethernet.h"
#include "arp.h"
#include "netdev.h"
#include "ipv4.h"
#include "tcp.h"
#include "cli.h"

#define NUM_THREADS 2

static pthread_t threads[NUM_THREADS];
int running = 1;

static struct command *cmd_to_run;

///// Signal related
extern void init_signals();


//extern void tcp_in(struct netdev *netdev, struct eth_hdr *hdr);
//extern void tcp_out(struct tcp_socket *sock, struct tcphdr *thdr);



static void init_stack() {
    
    netdev_init("10.0.0.4", "00:0c:29:6d:50:25");
    arp_init();
    tcp_init();
}




static void run_threads() {
    if(pthread_create(&threads[0], NULL, &netdev_loop, NULL) != 0) {
        print_error("Could not create netdev loop\n");
        return;
    }


    if(pthread_create(&threads[1], NULL, cmd_to_run->cmd_func, cmd_to_run) != 0) {
        print_error("Could not create app thread\n");
        return;
    }
    
    //return 0;
}

static void wait_threads() {
    printf("CLOSING THREADS\n");
    for(int i=0; i<NUM_THREADS; i++) {
        if(pthread_join(threads[i], NULL) != 0) {
            print_error("Error when joining threads");
            exit(1);
        }
        printf("CLOSED\n");
    }
    
}

int main(int argc, char** argv) {
    
    cmd_to_run = parse_args(argc, argv);
    
    init_signals();
    init_stack();
    
    
    run_threads();
    wait_threads();    


    netdev_free();
    return 0;
}
