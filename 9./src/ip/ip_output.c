#include "lib/syshead.h"
#include "lib/utils.h"
#include "ip/ip.h"
#include "tcp/tcp.h"
#include "arp/route.h"
#include "socket/sock.h"


int ip_output(struct sock *sk, struct sock_buff *skb) {
//int ip_output(struct tcp_socket *sock, struct sock_buff *skb) {
    struct route_table_t *rt;
    struct iphdr *hdr = ip_hdr(skb);
    
    rt = route_lookup(hdr->daddr);
    if(!rt) {perror("not found");}
   
    skb->dst = &rt->dst; 
    struct netdev *dev = skb->dst->dev;
    skb->netdev = dev;
    skb_push(skb, IP_HDR_LEN);
    
    
    hdr->version = IPV4;
    hdr->ihl = 0x05;
    hdr->tos = 0;
    hdr->len = htons(skb->size);
    hdr->id = htons(hdr->id);
    hdr->flags = 0;
    hdr->flag_offset = 0;
    hdr->ttl = 64;
    hdr->proto = skb->protocol; 
    hdr->saddr = dev->addr;
    hdr->daddr = sk->daddr;
    hdr->csum = 0;
    
    ip_send_check(hdr);
    return dst_nei_output(skb);
}


void ip_send_check(struct iphdr *hdr) {
    hdr->csum = checksum(hdr, hdr->ihl * 4, 0);
}




