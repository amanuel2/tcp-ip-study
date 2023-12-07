#include "arp/arp.h"
#include "socket/skbuff.h"

/*
 * https://tools.ietf.org/html/rfc826
 */

static struct arp_cache_entry arp_cache[ARP_CACHE_LEN];



int insert_arp_translation_table(struct arp_hdr *hdr, struct arp_ipv4 *data)
{
    struct arp_cache_entry *entry;
    for (int i = 0; i<ARP_CACHE_LEN; i++) {
        entry = &arp_cache[i];

        if (entry->state == ARP_FREE) {
            entry->state = ARP_RESOLVED;

            entry->hwtype = hdr->hwtype;
            entry->sip = data->sip;
            memcpy(entry->smac, data->smac, sizeof(entry->smac));

            return 0;
        }
    }

    return -1;
}


int update_arp_translation_table(struct arp_hdr *hdr, struct arp_ipv4 *data)
{
    struct arp_cache_entry *entry;

    for (int i = 0; i<ARP_CACHE_LEN; i++) {
        entry = &arp_cache[i];

        if (entry->state == ARP_FREE) continue;

        if (entry->hwtype == hdr->hwtype && entry->sip == data->sip) {
            memcpy(entry->smac, data->smac, 6);
            return 1;
        }
    }
    
    return 0;
}

void arp_init() {
    memset(arp_cache, 0, ARP_CACHE_LEN * sizeof(struct arp_cache_entry));
}

/*
 * Returns the HW address of the given source IP address
 * NULL if not found
 */
uint8_t* arp_get_hwaddr(uint32_t *sip) {
    
    struct arp_cache_entry *entry;

    for (int i = 0; i < ARP_CACHE_LEN; i++) {
        entry = &arp_cache[i];

        if (entry->state == ARP_RESOLVED && 
            entry->sip == sip) {

            return entry->smac;
        }
    }

    return NULL;
}


int arp_hdr_len(struct netdev *dev) {
    return sizeof(struct arp_hdr) + (dev->addr_len+sizeof(uint32_t)*2);
}