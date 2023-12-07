#ifndef TCP_H_
#define TCP_H_

#include "syshead.h"
#include "netdev.h"
#include "ipv4.h"

#define TCP_HDR_LEN 20

/*
                    Connection  (At end send data with SEQ #)
          TCP A                                                TCP B
    	 ------------------------------------------------------------- 
    1.   CLOSED                                               LISTEN
    	
    2.   SYN-SENT    --> <SEQ=100><CTL=SYN>               --> SYN-RECEIVED
    	  
    3.   ESTABLISHED <-- <SEQ=300><ACK=101><CTL=SYN,ACK>  <-- SYN-RECEIVED
    			
    4.   ESTABLISHED --> <SEQ=101><ACK=301><CTL=ACK>       --> ESTABLISHED
    			  
    5.   ESTABLISHED --> <SEQ=101><ACK=301><CTL=ACK><DATA> --> ESTABLISHED
*/

/*
        0                            15                              31
       -----------------------------------------------------------------
       |          source port (2)      |       destination port (2)    |
       -----------------------------------------------------------------
       |                        sequence number     (4)                |
       -----------------------------------------------------------------
       |                     acknowledgment number  (4)                |
       -----------------------------------------------------------------
       |  HL   | rsvd  |C|E|U|A|P|R|S|F|        window size   (2)      |  /// Left side(16 bits) = Data offset(4), Reserved(3), Control Flags (9) 
       -----------------------------------------------------------------
       |         TCP checksum (2)      |       urgent pointer (2)      |
       |---------------------------------------------------------------|
       |                     Optional Data (0-40)                      |
       -----------------------------------------------------------------
*/

struct tcphdr {
    uint16_t sport;
    uint16_t dport;
    uint32_t seq;
    uint32_t ack;
    uint8_t rsvd : 4;
    uint8_t hl : 4;
    uint8_t flags;
    uint16_t win;
    uint16_t csum;
    uint16_t urp;
    uint8_t data[];
} __attribute__((packed));



// Flags(rsvd) : remember first 4 bits not used
#define TCP_FIN 0x01
#define TCP_SYN 0x02
#define TCP_RST 0x04
#define TCP_PSH 0x08
#define TCP_ACK 0x10

#define TCP_URG 0x20
#define TCP_ECN 0x40
#define TCP_WIN 0x80

struct tcpiphdr {
    uint32_t saddr;
    uint32_t daddr;
    uint8_t zero;
    uint8_t proto;
    uint16_t tlen;
} __attribute__((packed));

void tcp_incoming(struct netdev * dev, struct eth_hdr *hdr);
void tcp_outgoing(struct netdev* dev, struct eth_hdr *hdr);

#endif