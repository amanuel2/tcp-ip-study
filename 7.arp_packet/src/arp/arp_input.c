#include "arp/arp.h"

static uint8_t broadcast_hw[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

static inline struct arp_hdr *arp_hdr(struct sock_buff *skb) {
    return (struct arp_hdr *) (skb->head + ETH_HDR_LEN);
}

/*

arp_rcv:

Purpose: The arp_rcv function is called to handle an incoming ARP packet. 
         This could be an ARP request or an ARP reply. 
         The function is part of the network stack's input path and is 
         nvoked when an ARP packet is received by the network interface.
         
Behavior: When an ARP packet is received, arp_rcv processes (request or reply).
        
        For an ARP request, it checks if the target IP address matches any of the system's 
        IP addresses. If it does, the system responds with an ARP reply.
        
        For an ARP reply, arp_rcv updates the ARP cache with the newly learned MAC address associated 
        with the IP address in the reply.
*/

void arp_rcv(struct sock_buff *skb) {
    
    struct arp_hdr *arphdr;
    struct arp_ipv4 *arpdata;
    int merge = 0;

    arphdr = arp_hdr(skb);

    arphdr->hwtype = ntohs(arphdr->hwtype);
    arphdr->protype = ntohs(arphdr->protype);
    arphdr->opcode = ntohs(arphdr->opcode);

    if (arphdr->hwtype != ARP_ETHERNET) {
        printf("Unsupported HW type\n");
        return;
    }

    if (arphdr->protype != ARP_IPV4) {
        printf("Unsupported protocol\n");
        return;
    }

    arpdata = (struct arp_ipv4 *) arphdr->data;

    merge = update_arp_translation_table(arphdr, arpdata);
    
    struct netdev *netdev;
    if (!(netdev = netdev_get(arpdata->dip))) {
        char str2[30];
        inet_ntop(AF_INET, &arpdata->dip, str2, 30);
        printf("ARP (Req, or Reply) was not for us, They are %s\n", str2);
        return;
    }

    if (!merge && insert_arp_translation_table(arphdr, arpdata) != 0) {
       perror("ERR: No free space in ARP translation table\n"); 
    }

    switch (arphdr->opcode) {
    case ARP_REQUEST:
        arp_reply(skb, netdev);
        break;
    default:
        printf("Opcode not supported\n");
        break;
    }
}

/*
arp_request:

Purpose: The primary purpose of arp_request is to send an ARP request.
This function is used when a network device needs to find the MAC (Media Access Control) 
address associated with an IP address on the local network. 
It's typically called when the system wants to send a packet to a local network address but 
doesn't know the corresponding MAC address.

Behavior: When invoked, arp_request constructs an ARP request packet. 
This packet contains the sender's IP and MAC addresses and the target IP 
address for which the MAC address is needed. The ARP request is broadcasted
over the network. The device on the network with the matching IP address responds 
with its MAC address in an ARP reply.
*/
int arp_request(uint32_t sip, uint32_t dip, struct netdev *dev) {
    struct sock_buff *skb;
    struct arp_hdr *arph;
    struct arp_ipv4 * payload;
    
    skb = alloc_skb(ETH_HDR_LEN + ARP_HDR_LEN + ARP_DATA_LEN);
    arph = arp_hdr(skb);
    
    // Broadcasting ARP Request to the network
    skb->protocol = htons(ETH_P_ARP);
    
    arph->hwtype = ARP_ETHERNET;
    arph->protype = ETH_P_IP;
    arph->hwsize = dev->addr_len;
    arph->prosize = 4; // IPv4
    
    
    
    payload = (struct arp_ipv4 *) arph->data;
    
    memcpy(payload->smac, dev->hwaddr, dev->addr_len);
    memcpy(payload->dmac, broadcast_hw, dev->addr_len);
    
    payload->sip = sip;
    payload->dip = dip;
    
    return netdev_transmit(skb, ETH_P_ARP, broadcast_hw);
}