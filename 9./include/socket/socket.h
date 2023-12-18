#ifndef  SOCKET_H_
#define  SOCKET_H_

#include "lib/syshead.h"
#include "socket/wait.h"

struct socket;

enum socket_state {
    SS_FREE = 0,                    /* not allocated                */
    SS_UNCONNECTED,                 /* unconnected to any socket    */
    SS_CONNECTING,                  /* in process of connecting     */
    SS_CONNECTED,                   /* connected to socket          */
    SS_DISCONNECTING                /* in process of disconnecting  */
};


struct sock_type {
    struct socket_ops *socket_ops;
    struct net_ops *net_ops;
    int type;
    int protocol;
};
// diff socket types
struct net_family {
    int (*create) (struct socket *sock, int protocol);
};


struct socket_ops {
    int (*connect) (struct socket *sk, struct socketaddr *vaddr, int len, int flags);
    int (*socket) (struct socket *sock, int protocol);
    
    int (*write) (struct socket *sock, const void *buf, int len);
    int (*read) (struct socket *sock, int len);
};


struct socket {
    uint32_t fd;
    enum socket_state state;
    uint16_t type;
    struct sock *sk;
    struct socket_ops *ops;
    struct wait_lock sleep;
};

int _socket(int domain, int type, int protocol);
int _connect(int socketfd, const struct sockaddr *addr, socklen_t addrlen);

#endif