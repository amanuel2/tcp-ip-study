#include "arp/dst.h"
#include "arp/route.h"


struct route_table_t main_rt;
extern struct netdev netdev;


void route_init() {
    struct dst_entry_t dst = { .dev = &netdev };
    main_rt.dst = dst;
}

struct route_table_t *route_lookup(uint32_t daddr) {
    return &main_rt;
}