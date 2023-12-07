#ifndef SOCK_H_
#define SOCK_H_

#include "socket.h"

struct sock;

struct net_ops {
    int (*connect) (struct sock *sk, struct sockaddr addr, int len);
};

struct sock {
    struct socket *sock;
    struct net_ops *ops;

    int protocol;
    int state;

    uint16_t sport;
    uint16_t dport;

    uint32_t saddr;
    uint32_t daddr;
};

struct sock *sk_alloc(struct net_ops *ops);
void sock_init_data(struct socket *sock, struct sock *sk);
#endif