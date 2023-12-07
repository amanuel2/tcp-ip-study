#include "socket/socket.h"

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
    
    struct tcp_socket *sock;
    if((sock = get_tcp_socket(sockfd)) == 0) {
        perror("Couldnt find socket for conn\n");
        exit(1);
    }
    
    return tcp_v4_init(sock, addr, addrlen);
}