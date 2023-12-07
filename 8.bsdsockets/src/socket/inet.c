#include "socket/inet.h"
#include "socket/sock.h"
#include "tcp/tcp.h"

extern struct net_ops tcp_ops;

static int INET_OPS = 1;

struct net_family inet = {
    .create = inet_create,
};

static struct socket_ops inet_stream_ops = {
    .connect = inet_stream_connect
};

// bridge forming here
static struct sock_type inet_ops[] = {
    {
        .socket_ops = &inet_stream_ops,
        .net_ops = &tcp_ops,
        .type = SOCK_STREAM,
        .protocol = IPPROTO_TCP
    }
};

int inet_create(struct socket *sock, int protcol) {
    struct sock *sk;
    struct sock_type *skt = 0;
    
    // going through all Network - Transport combinations
    for(int i=0; i<INET_OPS; i++) {
        if(inet_ops[i].type == sock->type) {
            skt = &inet_ops[i];
        }
    }
    
    if(!skt) {
        perror("Couldnt find socktype for socket\n");
        return 1;
    }
    
    sock->ops = skt->socket_ops;
    sk = sk_alloc(skt->net_ops);
    
    sock_init_data(sock, sk);
    sock->sk = sk;
    return 0;
}


int inet_socket(struct socket *sock, int protocol) {
    return 0;        
}

int inet_connect(struct socket *sock, struct sockaddr *addr, int len, int flags) {
    return 0;
}


int inet_stream_connect(struct socket *sock, struct sockaddr *addr, int len, int flags) {
    return 0;
}