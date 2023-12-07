#include "tcp.h"
#include "ipv4.h"
#include "utils.h"
#include "tcp_socket.h"


void tcp_init() {
    init_tcp_sockets();
}

void tcp_incoming(struct netdev* dev, struct eth_hdr* hdr) {
   
   struct iphdr *iphdr = (struct iphdr*) hdr->payload;
   struct tcphdr* tcphdr = (struct tcp_hdr*) iphdr->data;
   
   if(tcp_checksum(iphdr, tcphdr) != 0) {
       perror("TCP CHECKSUM ERR(NO MATCH)\n");
       return;
   }
   
   printf("Sucessfull TCP_IN bout to TCP_OUT\n");
   tcp_outgoing(dev, hdr);
    
}

void tcp_outgoing(struct netdev* dev, struct eth_hdr* hdr) {
    
   struct iphdr *iphdr = (struct iphdr*) hdr->payload;
   struct tcphdr* tcphdr = (struct tcp_hdr*) iphdr->data;
   
   struct iphdr pseudo_hdr;
   
   // simple swap
   uint16_t tmpport = tcphdr->sport;
   tcphdr->sport = tcphdr->dport;
   tcphdr->dport = tmpport;
   
   printf("Src Port: %hu Dest Port: %hu\n", ntohs(tcphdr->sport), ntohs(tcphdr->dport));
   
   // Connection (handshake)
    if(tcphdr->flags & TCP_SYN) {
        tcphdr->flags |= TCP_ACK;
        
        tcphdr->ack = htonl(ntohl(tcphdr->seq) + 1);
        tcphdr->seq = htonl(12345678); // TODO: generate rand later
    }
    
    /* Cut off TCP options, we'll implement the important ones later */
    tcphdr->hl = 5; // The value 5 indicates that the TCP header is 5 * 32 bits (or 20 bytes) long. 
    
    int data_length = 20; // where data_length is the length of the TCP payload (if any).
    iphdr->len = iphdr->len + data_length;
    //iphdr->len -= TCP_HDR_LEN;
    
   pseudo_hdr.saddr = iphdr->daddr;
   pseudo_hdr.daddr = iphdr->saddr;
   pseudo_hdr.proto = iphdr->proto;
   pseudo_hdr.len = iphdr->len;
   pseudo_hdr.ihl = iphdr->ihl;
   
   tcphdr->csum = 0;
   tcphdr->csum = tcp_checksum(&pseudo_hdr, tcphdr);
   
   printf("Sucessfull TCP_OUT Bout to tunnel it to IP\n");
   ipv4_outgoing(dev, hdr);
}