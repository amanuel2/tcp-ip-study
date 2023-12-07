#ifndef TCP_H_
#define TCP_H_

#include "syshead.h"
#include "netdev.h"
#include "ipv4.h"

#define TCP_HDR_LEN sizeof(struct tcphdr)

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
       |  HL   | rsvd  |C|E|U|A|P|R|S|F|        window size   (2)      |  /// Left side(16 bits) = Data offset(4), Reserved(6), Control Flags (6) 
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
    uint32_t ack_seq;
    uint8_t rsvd : 4;
    uint8_t hl : 4;
//    uint8_t fin : 1,
//            syn : 1,
//            rst : 1,
//            psh : 1,
//            ack : 1,
//            urg : 1,
//            ece : 1,
//            cwr : 1;
    uint8_t flags;
    uint16_t win;
    uint16_t csum;
    uint16_t urp;
    uint8_t data[];
} __attribute__((packed));

struct tcpiphdr {
    uint32_t saddr;
    uint32_t daddr;
    uint8_t zero;
    uint8_t proto;
    uint16_t tlen;
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






/////////////////////////// TCP SOCKET ///////////////////////////

struct tcp_socket;

enum tcp_states {
    CLOSED,
    LISTEN,
    SYN_SENT,
    SYN_RECEIVED,
    ESTABLISHED,
    FIN_WAIT_1,
    FIN_WAIT_2,
    CLOSE_WAIT,
    CLOSING,
    LAST_ACK
};


// TCP Has to keep track of requests of sequence of data so TCB (Transmission Control Block)
/*
 Send Sequence Variables
	
      SND.UNA - send unacknowledged
      SND.NXT - send next
      SND.WND - send window
      SND.UP  - send urgent pointer
      SND.WL1 - segment sequence number used for last window update
      SND.WL2 - segment acknowledgment number used for last window update
      ISS     - initial send sequence number
      
    Receive Sequence Variables
											  
      RCV.NXT - receive next
      RCV.WND - receive window
      RCV.UP  - receive urgent pointer
      IRS     - initial receive sequence number
      
    Current Segment Variables
	
      SEG.SEQ - segment sequence number
      SEG.ACK - segment acknowledgment number
      SEG.LEN - segment length
      SEG.WND - segment window
      SEG.UP  - segment urgent pointer
      SEG.PRC - segment precedence value
*/
struct tcb {
    uint8_t tcp_flags;
    uint8_t *snd_buf;
    uint8_t *rcv_buf;
    uint32_t snd_una;
    uint32_t snd_nxt;
    uint32_t snd_wnd;
    uint32_t snd_up;
    uint32_t snd_wl1;
    uint32_t snd_wl2;
    uint32_t iss;
    uint32_t rcv_nxt;
    uint32_t rcv_wnd;
    uint32_t rcv_up;
    uint32_t irs;
};

struct tcp_socket {
    enum tcp_states state;
    int fd;
    struct tcb tcb;
    
    uint16_t sport;
    uint16_t dport;
   
    uint32_t saddr; 
    uint32_t daddr;
    uint16_t hln;
};




void init_tcp_sockets();
void free_tcp_socket();

struct tcp_socket* alloc_tcp_socket();
struct tcp_socket* get_tcp_socket(int sockfd);


int tcp_v4_connect_init(struct tcp_socket *sock, const struct sockaddr *addr, socklen_t addrlen);
int tcp_connect(struct tcp_socket *sock);
//////////////////////////////////////////////////////



 //////////////////////// TCP /////////////////////



void tcp_input(struct netdev * dev, struct eth_hdr *hdr);
void tcp_output(struct tcp_socket *sock, struct tcphdr *hdr);

//void tcp_outgoing(struct netdev* dev, struct eth_hdr *hdr);
void tcp_init();

void tcp_select_initial_window(uint32_t *rcv_wnd);

////////////////////////////////////////////////


/// Utils
uint32_t generate_iss();
#endif