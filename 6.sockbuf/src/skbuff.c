#include "skbuff.h"

struct sock_buff *alloc_skb(uint32_t size) {
    struct sock_buff *skb = malloc(sizeof (struct sock_buff));
    
    skb->data = malloc(size);
    skb->head = skb->data;
    skb->tail = skb->data;
    skb->end = skb->tail+size;
    return skb;
}


uint8_t skb_push(struct sock_buff *skb, uint32_t len) {
    skb->data -= len;
    skb->tail += len;
    return skb->data;
}


void* skb_reserve(struct sock_buff *skb, uint32_t len) {
    skb->data += len;
    skb->tail += len;
    return skb->data;
}