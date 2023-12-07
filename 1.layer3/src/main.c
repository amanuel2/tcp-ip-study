#include "syshead.h"
#include "basic.h"
#include "tuntap_if.h"
#include "utils.h"
#include "ethernet.h"
#include "arp.h"
#include "netdev.h"
#include "ipv4.h"

#define BUFLEN 100

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
        default:
            printf("Unrecognized ethertype %x\n", hdr->ethertype);
            break;
    }
}

int main(int argc, char** argv)
{
    char buf[BUFLEN];
    char *dev = calloc(10, 1);
    struct netdev netdev;

    CLEAR(buf);

    tun_init(dev);
    netdev_init(&netdev, "10.0.0.5", "86:2f:6f:cc:d8:9d");

    arp_init();

    while (1) {
        printf("START\n");
        if (tun_read(buf, BUFLEN) < 0) {
            print_error("ERR: Read from tun_fd: %s\n", strerror(errno));
            return 1;
        }
        printf("END\n");

        print_hexdump(buf, BUFLEN);

        struct eth_hdr *hdr = init_eth_hdr(buf);

        handle_frame(&netdev, hdr);
    }
    
    
    free(dev);
}
