#include "syshead.h"
#include "basic.h"
#include "ethernet.h"


struct eth_hdr* init_eth_hdr(char* buf)
{
    struct eth_hdr *hdr = (struct eth_hdr *) buf;

    hdr->ethertype = htons(hdr->ethertype);

    return hdr;
}

void handle_frame(struct netdev *netdev, struct eth_hdr *hdr) {
    switch (hdr->ethertype) {
        case ETH_P_ARP:
            printf("ETH ARP REQ\n");
            arp_incoming(netdev, hdr);
            break;
        case ETH_P_IP:
            printf("ETH IP REQ\n");
            ipv4_input(netdev, hdr);
            break;
        case ETH_P_IPV6:
            //printf("IPV6 Isn't implemented yet\n");
            break;
        default:
            printf("Unrecognized ethertype %x\n", hdr->ethertype);
            break;
    }
}