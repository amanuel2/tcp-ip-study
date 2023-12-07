#ifndef NETDEV_H
#define NETDEV_H
#include "lib/syshead.h"
#include "eth/eth.h"
#include "socket/skbuff.h"

#define BUFLEN 512

struct netdev {
    uint32_t addr;
    uint8_t hwaddr[6];
    uint8_t addr_len;
   
};

void netdev_init(char *addr, char *hwaddr);
int netdev_transmit(struct sock_buff *skb, uint16_t ethertype, uint8_t *dst);

void* netdev_loop();
int netdev_rx_action(struct sock_buff *skb);
void netdev_free();


struct netdev *netdev_get(uint32_t sip);
#endif
