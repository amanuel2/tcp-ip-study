#include "lib/syshead.h"
#include "socket/skbuff.h"



struct sock_buff *alloc_skb(uint32_t size) {
    struct sock_buff *skb = malloc(sizeof (struct sock_buff));
    
    memset(skb, 0, sizeof(struct sock_buff));
    skb->data = malloc(size);
    memset(skb->data, 0, size);
    
    skb->head = skb->data;
    skb->end = skb->data+size;
    return skb;
}


uint8_t* skb_push(struct sock_buff *skb, uint32_t size) {
    skb->data -= size;
    skb->size += size;
    return skb->data;
}


/**
 *	skb_reserve - adjust headroom
 *	@skb: buffer to alter
 *	@len: bytes to move
 *
 *	Increase the headroom of an empty &sk_buff by reducing the tail
 *	room. This is only allowed for an empty buffer.
 */
void* skb_reserve(struct sock_buff *skb, unsigned int len) {
	skb->data += len;
	// skb->tail += len;
	return skb->data;
}


void free_skb(struct sock_buff *skb) {
    free(skb);
    free(skb->data);
}



#include "arp/dst.h"
#include "dev/netdev.h"
void print_skb(struct sock_buff *skb) {
    if (!skb) {
        printf("skb is NULL\n");
        return;
    }

    printf("skb contents:\n");
    printf("Protocol: 0x%x\n", skb->protocol);
    printf("Size: %u\n", skb->size);

    if (skb->dst && skb->dst->dev) {
        struct netdev *dev = skb->dst->dev;
               char str_addr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &dev->addr, str_addr, INET_ADDRSTRLEN);
    printf("Dev Address: %s\n", str_addr);
        printf("Dev hwaddr: ");
        for (int i = 0; i < 6; i++) {
            printf("%02x", dev->hwaddr[i]);
            if (i < 5) printf(":");
        }
        printf("\n");
    } else {
        printf("skb->dst or skb->dst->dev is NULL\n");
    }

    printf("Data pointer: %p\n", (void *)skb->data);
    printf("Head pointer: %p\n", (void *)skb->head);
    printf("End pointer: %p\n", (void *)skb->end);

    printf("Data content: ");
    for (uint8_t *ptr = skb->head; ptr < skb->end; ptr++) {
        printf("%02x ", *ptr);
    }
    printf("\n");
        printf("Data content as text: ");
    for (uint8_t *ptr = skb->head; ptr < skb->end; ptr++) {
        if (isprint(*ptr)) {
            printf("%c", *ptr);
        } else {
            printf(".");
        }
    }
    printf("\n");
    printf("\n");
}