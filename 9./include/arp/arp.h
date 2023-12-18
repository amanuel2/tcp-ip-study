#ifndef ARP_H
#define ARP_H
#include "lib/syshead.h"
#include "eth/eth.h"
#include "dev/netdev.h"
#include "socket/skbuff.h"

#define ARP_ETHERNET    0x0001
#define ARP_IPV4        0x0800
#define ARP_REQUEST     0x0001
#define ARP_REPLY       0x0002

#define ARP_CACHE_LEN   32
#define ARP_FREE        0
#define ARP_WAITING     1
#define ARP_RESOLVED    2

#define ARP_HDR_LEN sizeof(struct arp_hdr)
#define ARP_DATA_LEN sizeof(struct arp_ipv4)

#define arp_dbg(str, hdr)                                               \
    do {                                                                \
        print_debug("arp "str" (hwtype: %hu, protype: %.4hx, "          \
                    "hwsize: %d, prosize: %d, opcode: %.4hx)",          \
                    hdr->hwtype, hdr->protype, hdr->hwsize,             \
                    hdr->prosize, hdr->opcode);                         \
    } while (0)

#define arpdata_dbg(str, data)                                          \
    do {                                                                \
        print_debug("arp data "str" (smac: %.2hhx:%.2hhx:%.2hhx:%.2hhx"  \
                    ":%.2hhx:%.2hhx, sip: %hhu.%hhu.%hhu.%hhu, dmac: %.2hhx:%.2hhx" \
                    ":%.2hhx:%.2hhx:%.2hhx:%.2hhx, dip: %hhu.%hhu.%hhu.%hhu)", \
                    data->smac[0], data->smac[1], data->smac[2], data->smac[3], \
                    data->smac[4], data->smac[5], data->sip >> 24, data->sip >> 16, \
                    data->sip >> 8, data->sip >> 0, data->dmac[0], data->dmac[1], \
                    data->dmac[2], data->dmac[3], data->dmac[4], data->dmac[5], \
                    data->dip >> 24, data->dip >> 16, data->dip >> 8, data->dip >> 0); \
    } while (0)

#define arpcache_dbg(str, entry) \
    do { \
    print_debug("arp cache "str" (hwtype: %hu, sip: %hhu.%hhu.%hhu.%hhu, " \
    "smac: %.2hhx:%.2hhx:%.2hhx:%.2hhx:%.2hhx:%.2hhx, state: %d)", entry->hwtype, \
        entry->sip >> 24, entry->sip >> 16, entry->sip >> 8, entry->sip >> 0, \
        entry->smac[0], entry->smac[1], entry->smac[2], entry->smac[3], entry->smac[4], \
                entry->smac[5], entry->state); \
    } while (0)


struct netdev;

struct arp_hdr {
    uint16_t hwtype;
    uint16_t protype;
    uint8_t hwsize;
    uint8_t prosize;
    uint16_t opcode;
    unsigned char data[];
} __attribute__((packed));

/// For ARP Requests
struct arp_ipv4 {
    unsigned char smac[6];
    uint32_t sip;
    unsigned char dmac[6];
    uint32_t dip;
} __attribute__((packed));



/// @brief Arp Table
struct arp_cache_entry {
    uint16_t hwtype;
    uint32_t sip;
    unsigned char smac[6];
    unsigned int state;
};

void arp_init();
void arp_rcv(struct sock_buff *skb);
void arp_reply(struct sock_buff *skb,struct netdev *netdev);


int arp_hdr_len(struct netdev *dev);
int arp_request(uint32_t sip, uint32_t dip, struct netdev *dev);
uint8_t *arp_get_hwaddr(uint32_t *sip);


int insert_arp_translation_table(struct arp_hdr *hdr, struct arp_ipv4 *data);
int update_arp_translation_table(struct arp_hdr *hdr, struct arp_ipv4 *data);
#endif
