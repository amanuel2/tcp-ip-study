#include "arp/arp.h"

static inline struct arp_hdr *arp_hdr(struct sock_buff *skb) {
    return (struct arp_hdr *) (skb->head + ETH_HDR_LEN);
}

void arp_reply(struct sock_buff *skb, struct netdev *netdev) {
    
    struct arp_hdr  *arphdr;
    struct arp_ipv4 *arpdata;
    int len;

    arphdr = arp_hdr(skb);
    
    skb_reserve(skb, ETH_HDR_LEN + ARP_HDR_LEN + ARP_DATA_LEN);
    skb_push(skb, ARP_HDR_LEN + ARP_DATA_LEN);

    memcpy(arpdata->dmac, arpdata->smac, 6);
    arpdata->dip = arpdata->sip;
    memcpy(arpdata->smac, netdev->hwaddr, 6);
    arpdata->sip = netdev->addr;

    arphdr->opcode = ARP_REPLY;

    arphdr->opcode = htons(arphdr->opcode);
    arphdr->hwtype = htons(arphdr->hwtype);
    arphdr->protype = htons(arphdr->protype);
    
    skb->netdev = netdev;
    netdev_transmit(skb, ETH_P_ARP, arpdata->dmac);
}