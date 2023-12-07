#ifndef DEF_H_
#define DEF_H_

#include "lib/syshead.h"
#include "socket/skbuff.h"
#include "arp/arp.h"


struct dst_entry_t {
    struct netdev *dev;
};


int dst_nei_output(struct sock_buff *skb);

#endif