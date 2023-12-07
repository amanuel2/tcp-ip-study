#ifndef SKBUFF_H_
#define SKBUFF_H_
#include "syshead.h"

struct sock_buff {
    uint32_t size;
    uint8_t *tail;
    uint8_t *head;
    uint8_t *end;
    uint8_t *data;
} __attribute__((packed));

struct sock_buff *alloc_skb(uint32_t size);


uint8_t skb_push(struct sock_buff *skb, uint32_t len);
void* skb_reserve(struct sock_buff *skb, uint32_t len);
#endif