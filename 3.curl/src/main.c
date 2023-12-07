#include "syshead.h"
#include "basic.h"
#include "tuntap_if.h"
#include "utils.h"
#include "ethernet.h"
#include "arp.h"
#include "netdev.h"
#include "ipv4.h"
#include "curl.h"

#define BUFLEN 100

typedef void (*sighandler_t)(int);
static void usage(int argc, char** argv);
extern void curl(int, char**);


int running = 1;
static void stop_stack_handler(int signo) {running = 0;}
static int _signal(int signo, sighandler_t handler) {
    char s[200];
    struct sigaction sa = {
        .sa_handler = handler, 
        .sa_flags = SA_RESTART
    };
    
    if(sigaction(signo, &sa, NULL) < 0) return SIG_ERR;
    return 0;
}

/*inline*/ static void init_signals() {
    _signal(SIGINT, stop_stack_handler); // CTRL^C
}




void handle_frame(struct netdev *netdev, struct eth_hdr *hdr)
{
    switch (hdr->ethertype) {
        case ETH_P_ARP:
            printf("ETH ARP REQ\n");
            arp_incoming(netdev, hdr);
            break;
        case ETH_P_IP:
            printf("ETH IP REQ\n");
            ipv4_incoming(netdev, hdr);
            break;
        case ETH_P_IPV6:
            printf("IPV6 Isn't implemented yet\n");
            break;
        default:
            printf("Unrecognized ethertype %x\n", hdr->ethertype);
            break;
    }
}


inline static void usage(int argc, char** argv) {
    printf("Usage: sudo %s [curl HOST]\n\n", argv[0]);
    printf("  curl HOST - act like curl, HOST as the target. Optional.\n");
    printf("\n");
    printf("  Elevated privileges are needed because of tuntap devices.\n");
    exit(1);
}

struct command {
    int args;
    void (* cmd_func) (int, char **);
    char* cmd_str;
};

struct command cmds[] = {
    { 0, usage, "help" },
    { 1, curl, "curl" },
    { 0, NULL, NULL }
};




void parse_args(int argc, char** argv) {
    if(argc == 1)  return;
    struct command *cmd;
    
    for(cmd = &cmds[0]; cmd->cmd_func; cmd++) {
        if(strncmp(argv[1], cmd->cmd_str, 5) == 0) {
            cmd->cmd_func(argc, argv);
            return;
        }
    }
    
    usage(argc, argc);
}


int main(int argc, char** argv)
{
    char buf[BUFLEN];
    char *dev = calloc(10, 1);
    struct netdev netdev;

    CLEAR(buf);
    
    
    parse_args(argc, argv);
    
    init_signals();
    tun_init(dev);
    netdev_init(&netdev, "10.0.0.4", "86:2f:6f:cc:d8:9d");

    arp_init();

    while (running) {
        //printf("START\n");
        if (tun_read(buf, BUFLEN) < 0) {
            print_error("ERR: Read from tun_fd: %s\n", strerror(errno));
            return 1;
        }
        //printf("END\n");

        // print_hexdump(buf, BUFLEN);

        struct eth_hdr *hdr = init_eth_hdr(buf);

        handle_frame(&netdev, hdr);
    }
    
    
    free(dev);
}
