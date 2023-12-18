#ifndef SKBUFF_H_
#define SKBUFF_H_


struct sock_buff {
    struct dst_entry_t *dst;
    uint16_t protocol;
    struct netdev *netdev;
    uint32_t size;
    uint8_t *head;
    uint8_t *end;
    uint8_t *data;
};

struct sock_buff *alloc_skb(uint32_t size);
void print_skb(struct sock_buff *skb);

uint8_t* skb_push(struct sock_buff *skb, uint32_t len);
void* skb_reserve(struct sock_buff *skb, unsigned int len);

void free_skb(struct sock_buff *skb);

#endif