#include "lib/syshead.h"
#include "lib/basic.h"
#include "dev/tuntap_if.h"
#include "lib/utils.h"
#include "eth/eth.h"
#include "arp/arp.h"
#include "dev/netdev.h"
#include "ip/ip.h"
#include "tcp/tcp.h"
#include "apps/cli.h"
#include "arp/route.h"

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
    
    route_init();
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


/*
### tun_write
#0  0x0000555555556480 in netdev_queue_xmit ()
#1  0x000055555555606c in dst_nei_output ()
#2  0x0000555555556c71 in ipv4_output ()
#3  0x0000555555556dff in ip_queue_xmit ()
#4  0x00005555555579f4 in tcp_transmit_sockbuf (sock=0x55555555b5c0 <tcp_sockets>, buff=0x7ffff0001fd0)
    at src/tcp/tcp_output.c:71
#5  0x0000555555557a68 in tcp_send_syn (sock=0x55555555b5c0 <tcp_sockets>) at src/tcp/tcp_output.c:85
#6  0x0000555555557b15 in tcp_connect (sock=0x55555555b5c0 <tcp_sockets>) at src/tcp/tcp_output.c:108
#7  0x000055555555787b in tcp_v4_init ()
#8  0x0000555555557663 in _connect ()
curl()
*/

int main(int argc, char** argv) {
    
    cmd_to_run = parse_args(argc, argv);
    
    init_signals();
    init_stack();
    
    
    run_threads();
    wait_threads();    


    //netdev_free();
    return 0;
}
