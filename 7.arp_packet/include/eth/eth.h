#ifndef ETHERNET_H_
#define ETHERNET_H_

#define ETH_HDR_LEN sizeof(struct eth_hdr)

//#include <linux/if_ether.h>
#include "dev/netdev.h"

#include "socket/skbuff.h"
#include "lib/syshead.h"

struct eth_hdr 
{
    uint8_t  dmac[6];
    uint8_t  smac[6];
    uint16_t ethertype;
    uint8_t  payload[];
} __attribute__((packed));


struct eth_hdr* init_eth_hdr(char* buf);

void handle_frame(struct netdev *netdev, struct eth_hdr *hdr);


struct eth_hdr* eth_hdr(struct sock_buff *skb);
#endif
