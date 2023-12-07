#include "socket/socket.h"
#include "tcp/tcp.h"

static struct socket sockets[12];

extern struct net_family inet; // net family


static struct net_family *families[128] = {
    [AF_INET] = &inet,
};

static struct socket *alloc_socket() {
    struct socket* sock = &sockets[0];
    sock->fd = 5;
    sock->state = SS_UNCONNECTED;
    wait_init(&sock->sleep);
    return sock;
}


int _socket(int domain, int type, int protocol) {
    struct socket *sock;
    
    if((sock = alloc_socket()) == 0) {
        perror("Could not allocate a socket\n");
        exit(1);
    }
    
    
    printf("//~~~ CREATING A SOCKET ~~~//\n");
    printf("domain %x\n", domain);
    printf("type %x\n", type);
    printf("protocol %x\n", protocol);
    
    sock->type = type;
    families[domain]->create(sock, protocol);
    
    return sock->fd;
}

static struct socket* get_socket(int fd) {return &sockets[0];}

int _connect(int socketfd, const struct sockaddr *addr, socklen_t addrlen) {
    
    struct socket *sock;
    if((sock = get_socket(socketfd)) == 0) {
        perror("Couldnt find socket for conn\n");
        exit(1);
    }
    
    // sock->type = type;
    
    return tcp_v4_init(sock, addr, addrlen);
}