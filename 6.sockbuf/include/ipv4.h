#ifndef IPV4_H
#define IPV4_H
#include "syshead.h"

#include "netdev.h"
#include "skbuff.h"
#include "tcp.h"

#define IPV4 0x04
#define IP_TCP 0x06

#define ICMPV4 0x01

#define IP_HDR_LEN sizeof(struct iphdr)

struct iphdr {
    uint8_t ihl : 4; /* TODO: Support Big Endian hosts */
    uint8_t version : 4;
    uint8_t tos;
    uint16_t len;
    uint16_t id;
    uint16_t flags : 3;
    uint16_t frag_offset : 13;
    uint8_t ttl;
    uint8_t proto;
    uint16_t csum;
    uint32_t saddr;
    uint32_t daddr;
    uint8_t data[];
} __attribute__((packed));


void ipv4_input(struct netdev *netdev, struct eth_hdr *hdr);
void ipv4_output(struct netdev *netdev, struct eth_hdr *hdr);


static inline struct iphdr *ip_hdr (struct sock_buff *skb) {
    return (struct iphdr*) skb->data;
}


int ip_queue_xmit(struct tcp_socket *sock, struct sock_buff *skb);
#endif
