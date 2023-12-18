#include "lib/syshead.h"
#include "lib/basic.h"
#include "eth/eth.h"


// extern inline struct eth_hdr *eth_hdr(struct sock_buff *skb);
// struct eth_hdr* init_eth_hdr(char* buf)
// {
//     struct eth_hdr *hdr = (struct eth_hdr *) buf;
// 
//     hdr->ethertype = htons(hdr->ethertype);
// 
//     return hdr;
// }
// 

struct eth_hdr* eth_hdr(struct sock_buff *skb) {
    struct eth_hdr *hdr = (struct eth_hdr *)skb->head;
    
    hdr->ethertype = ntohs(hdr->ethertype);
    return hdr;
}