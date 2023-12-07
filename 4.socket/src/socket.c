#include "socket.h"

int _socket(int domain, int type, int protocol) {
    struct tcp_socket *sock;
    
    if((sock = alloc_tcp_socket()) == 0) {
        perror("Could not allocate a socket\n");
        exit(1);
    }
    
    printf("domain %x\n", domain);
    printf("type %x\n", type);
    printf("protocol %x\n", protocol);

    return sock->fd;
}


int _connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    
    printf("trying to initiate handshake with sock fd %d\n");
    struct tcp_socket *sock;
    
    uint16_t dport = addr->sa_data[1];
    if((sock = get_tcp_socket(sockfd)) == 0) {
        perror("Could not find a socker for connection\n");
        exit(1);
    }
    
    printf("Connecting socket %d to %hhu.%hhu.%hhu.%hhu\n", sockfd, addr->sa_data[2], addr->sa_data[3], addr->sa_data[4], addr->sa_data[5]);
    return 0;
}