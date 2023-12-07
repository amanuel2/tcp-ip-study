#include "tcp/tcp.h"
#include "lib/utils.h"
#include "socket/sock.h"

#define MAX_tcp_sockS 128

#define  FIRST_FD 3

struct net_ops tcp_ops = {
    .connect = &tcp_connect,
};

static struct tcp_sock tcp_socks[MAX_tcp_sockS];

static uint16_t generate_port() {return 12000;}
uint32_t generate_iss() {return 1525252;}

void tcp_init() {
    init_tcp_socks();
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




void init_tcp_socks() {
    // Try an upgraded CLEAR() with another param
    memset(tcp_socks, 0, sizeof(struct tcp_sock) * MAX_tcp_sockS);
}

struct sock* alloc_tcp_sock() {
    struct tcp_sock *tsk = malloc(sizeof(struct tcp_sock));
    return (struct sock *) tsk;
}

void free_tcp_sock(struct tcp_sock *sock) {
    tcp_socks[sock->fd - FIRST_FD].fd = 0;
}

struct tcp_sock* get_tcp_sock(int sockfd) {
    struct tcp_sock *sock = &tcp_socks[sockfd - FIRST_FD];
    if(sock->fd == 0) return 0;
    return sock;
}

// initalizing a socket
int tcp_v4_init(struct sock *sk, const struct sockaddr *addr, socklen_t addrlen, int flags) {
    
    /*
        Curosity 
        ---
        sa_family_t sin_family: Address family (e.g., AF_INET for IPv4).
        in_port_t sin_port: Port number (16 bits).
        struct in_addr sin_addr: IP address (32 bits).
        char sin_zero[8]: Padding to make the structure the same size as struct sockaddr.
    */
    
    // generating new sock (in the param)
    sk->sport = generate_port(); 
    sk->dport = addr->sa_data[1]; // grab the dest from addr (in the params)
    sk->saddr = parse_ipv4_string("10.0.0.4");
    sk->daddr = ((struct sockaddr_in *) addr)->sin_addr.s_addr;  // we just added a new field to a tcp socket (ip 😘)
    
    printf("Connecting socket to %hhu.%hhu.%hhu.%hhu:%d\n", addr->sa_data[2], addr->sa_data[3], addr->sa_data[4], addr->sa_data[5], addr->sa_data[1]);
    
    return tcp_connect(sk);
}


////////////////////////////////////////////////////////////

