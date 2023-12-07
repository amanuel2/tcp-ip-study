#include "syshead.h"
#include "utils.h"
#include "ipv4.h"
#include "tcp.h"
#include "arp.h"

void ipv4_output(struct netdev *netdev, struct eth_hdr *hdr) {
    struct iphdr *iphdr = (struct iphdr *)hdr->payload;
    uint32_t tmpaddr;
    uint16_t csum;
    uint8_t len = iphdr->len;

    /* Just swap the source and destination IP addresses,
     * don't bother with ARP lookup just now
     */
    tmpaddr = iphdr->saddr;
    iphdr->daddr = tmpaddr;
    iphdr->saddr = netdev->addr;

    /*
     * Switch back the necessary fields to Network Byte Order
     */
    iphdr->len = htons(iphdr->len);

    /* Calculate and set datagram checksum */
    iphdr->csum = 0;
    csum = checksum(iphdr, iphdr->ihl * 4, 0);
    iphdr->csum = csum;

    netdev_transmit(netdev, hdr, ETH_P_IP, len, hdr->smac);
}


int ip_send_check(struct iphdr *hdr) {}

int ip_queue_xmit(struct tcp_socket *sock, struct sock_buff *buff) {
    struct iphdr *hdr;
    skb_push(buff, IP_HDR_LEN);
    
    hdr = ip_hdr(buff);
    
    hdr->saddr = htons(sock->saddr);
    hdr->daddr = htons(sock->daddr);
    hdr->len = htons(IP_HDR_LEN);
    hdr->csum = 0;
    return neigh_resolve_output(buff);
}


