#include "tcp_socket.h"

#define MAX_TCP_SOCKETS 128

static int cur_fd = 3;
static struct tcp_socket tcp_sockets[MAX_TCP_SOCKETS];

/*

Steps 
-----

1) A socket is opened with socket, and the type is specified as IPv4/TCP.

2) connect launches the TCP handshake. Destination address and port are passed to the function.

3) After the connection is successfully established, sendto(3) is used to write data to the socket - in this case, the usual HTTP GET request.

4) From that point on, curl eventually reads the incoming data with recvfrom.

*/

void init_tcp_sockets() {
    // Try an upgraded CLEAR() with another param
    memset(tcp_sockets, 0, sizeof(struct tcp_socket) * MAX_TCP_SOCKETS);
}

void free_tcp_sockets() {
    
}

struct tcp_socket* alloc_tcp_socket() {
    struct tcp_socket *sock;
    for(int i=0; i<MAX_TCP_SOCKETS; i++) {
        sock = &tcp_sockets[i];
        if(sock->fd == 0) {
            sock->fd = cur_fd++;
            sock->state = CLOSED;
            return sock;
        }
    }
    return 0;
}


struct tcp_socket* get_tcp_socket(int sockfd) {
    struct tcp_socket *sock;
    for(int i=0; i<MAX_TCP_SOCKETS; i++) {
        sock = &tcp_sockets[i];
        if(sock->fd == sockfd) return sock;
    }
    return 0;
}