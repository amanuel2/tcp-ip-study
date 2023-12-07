#include "tcp/tcp.h"
#include "lib/utils.h"


#define MAX_TCP_SOCKETS 128

#define  FIRST_FD 3

static struct tcp_socket tcp_sockets[MAX_TCP_SOCKETS];

static uint16_t generate_port() {return 12000;}
uint32_t generate_iss() {return 1525252;}

void tcp_init() {
    init_tcp_sockets();
}


void tcp_in(struct sock_buff *skb) {
//   struct iphdr *iphdr = (struct iphdr*) hdr->payload;
//   struct tcphdr* tcphdr = (struct tcphdr*) iphdr->data;
   
//   if(tcp_checksum(iphdr, tcphdr) != 0) {
//       printf("TCP CHECKSUM ERR(NO MATCH)\n");
//       return;
//   }
   
  //printf("Sucessfull TCP_IN bout to TCP_OUT\n");
  //tcp_outgoing(dev, hdr);
    
}

//////////////////////// TCP SOCKETS ////////////////////////




void init_tcp_sockets() {
    // Try an upgraded CLEAR() with another param
    memset(tcp_sockets, 0, sizeof(struct tcp_socket) * MAX_TCP_SOCKETS);
}

struct tcp_socket* alloc_tcp_socket() {
    struct tcp_socket *sock;
    for(int i=0; i<MAX_TCP_SOCKETS; i++) {
        sock = &tcp_sockets[i];
        if(sock->fd == 0) {
            sock->fd = i+FIRST_FD;
            sock->state = CLOSED;
            return sock;
        }
    }
    return 0;
}

void free_tcp_socket(struct tcp_socket *sock) {
    tcp_sockets[sock->fd - FIRST_FD].fd = 0;
}

struct tcp_socket* get_tcp_socket(int sockfd) {
    struct tcp_socket *sock = &tcp_sockets[sockfd - FIRST_FD];
    if(sock->fd == 0) return 0;
    return sock;
}

// initalizing a socket
int tcp_v4_init(struct tcp_socket *sock, const struct sockaddr *addr, socklen_t addrlen) {
    
    /*
        Curosity 
        ---
        sa_family_t sin_family: Address family (e.g., AF_INET for IPv4).
        in_port_t sin_port: Port number (16 bits).
        struct in_addr sin_addr: IP address (32 bits).
        char sin_zero[8]: Padding to make the structure the same size as struct sockaddr.
    */
    
    // generating new sock (in the param)
    sock->sport = generate_port(); 
    sock->dport = addr->sa_data[1]; // grab the dest from addr (in the params)
    sock->saddr = parse_ipv4_string("10.0.0.4");
    sock->daddr = ((struct sockaddr_in *) addr)->sin_addr.s_addr;  // we just added a new field to a tcp socket (ip 😘)
    
printf("Connecting socket to %hhu.%hhu.%hhu.%hhu:%d\n", addr->sa_data[2], addr->sa_data[3], addr->sa_data[4], addr->sa_data[5], addr->sa_data[1]);
    
    return tcp_connect(sock);
}


////////////////////////////////////////////////////////////

