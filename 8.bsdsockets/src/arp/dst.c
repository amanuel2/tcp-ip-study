#include "arp/dst.h"
#include "dev/netdev.h"
#include "ip/ip.h"
#include "arp/arp.h"

int dst_nei_output(struct sock_buff *skb) {
    
    struct iphdr *iphdr = ip_hdr(skb);
    struct netdev *dev = skb->netdev;
    uint8_t *dmac = arp_get_hwaddr(iphdr->daddr);
    
    if(dmac) {
        return netdev_transmit(skb, ETH_P_IP, dmac);
    } else {
        int rc = arp_request(iphdr->saddr, iphdr->daddr, dev);
        free_skb(skb);
        return rc;
    }
    
}