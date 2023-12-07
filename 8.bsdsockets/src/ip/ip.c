#include "ip/ip.h"

struct iphdr *ip_hdr (struct sock_buff *skb) {
    return (struct iphdr*) (skb->head + ETH_HDR_LEN);
}