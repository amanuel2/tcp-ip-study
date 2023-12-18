#ifndef ROUTE_H_
#define ROUTE_H_

struct route_table_t {
    struct dst_entry_t dst;
};

void route_init();
struct route_table_t *route_lookup(uint32_t daddr);

#endif