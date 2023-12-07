#include "syshead.h"
#include "utils.h"
#include "tcp.h"

static struct sock_buff *tcp_alloc_skb(int size) {
    struct sock_buff *skb = alloc_skb(size + ETH_HDR_LEN + IP_HDR_LEN + TCP_HDR_LEN); //// 😫 Uhhhh Purrr
    skb_reserve(skb, ETH_HDR_LEN + IP_HDR_LEN + TCP_HDR_LEN);
    return skb;
}

/*
      EXAMPLE
      
       Connection
       ----
       IP 10.0.0.4.12000 > 10.0.0.5.8000: Flags [S], seq 1525252, win 29200, length 0
       IP 10.0.0.5.8000 > 10.0.0.4.12000: Flags [S.], seq 825056904, ack 1525253, win 29200, options [mss 1460], length 0
       IP 10.0.0.4.12000 > 10.0.0.5.8000: Flags [.], ack 1, win 29200, length 0
       
       Sending Data
       ---
       IP 10.0.0.5.8000 > 10.0.0.4.12000: Flags [P.], seq 1:18, ack 18, win 29200, length 17
       IP 10.0.0.4.12000 > 10.0.0.5.8000: Flags [.], ack 18, win 29200, length 0
       IP 10.0.0.5.8000 > 10.0.0.4.12000: Flags [P.], seq 18:156, ack 18, win 29200, length 138
       IP 10.0.0.4.12000 > 10.0.0.5.8000: Flags [.], ack 156, win 29200, length 0
       IP 10.0.0.5.8000 > 10.0.0.4.12000: Flags [P.], seq 156:374, ack 18, win 29200, length 218
       IP 10.0.0.4.12000 > 10.0.0.5.8000: Flags [.], ack 374, win 29200, length 0
       
       
       Close Connection
       ---
       
       IP 10.0.0.5.8000 > 10.0.0.4.12000: Flags [F.], seq 374, ack 18, win 29200, length 0
       IP 10.0.0.4.12000 > 10.0.0.5.8000: Flags [.], ack 375, win 29200, length 0
       Host B informs host A that it has no more data to send by generating a FIN segment. In turn, host A acknowledges this.
        
       To finish the connection, host A also has to signal that it has no more data to send.
*/

static int tcp_transmit_sockbuf(struct tcp_socket *sock, struct sock_buff *buff) {
    struct tcphdr *tcphdr = (struct tcphdr *) buff->data;
    struct tcb *tcb = &sock->tcb;
    
    skb_push(sock, sock->hln);
    
    tcphdr->sport = sock->sport;
    tcphdr->dport = sock->dport;
    
    tcphdr->seq = sock->tcb.snd_nxt;
    tcphdr->ack_seq = sock->tcb.rcv_nxt;    
    
    tcphdr->rsvd = 0;
    tcphdr->hl = 6;
    
    // Set individual flags
    // tcphdr.fin = 0;
    // tcphdr.syn = 1; // SYN flag set
    // tcphdr.rst = 0;
    // tcphdr.psh = 0;
    // tcphdr.ack = 0;
    // tcphdr.urg = 0;
    // tcphdr.ece = 0;
    // tcphdr.cwr = 0;
    tcphdr->flags = tcb->tcp_flags;
    
    
    tcphdr->win = sock->tcb.rcv_wnd;
    tcphdr->csum = 0;
    tcphdr->urp = 0;
    
    
    return ip_queue_xmit(sock, buff);
}

static int tcp_send_syn(struct tcp_socket *sock) {
    
    if(sock->state != CLOSED && sock->state != LISTEN) {
        perror("Socket was not in correct state (closed | listen)\n");
        return 1;
    }
   
    struct sock_buff *skb = alloc_skb(0); // this line is so purrfect 😫
    
    sock->state = SYN_SENT;
    
    return  tcp_transmit_sockbuf(sock, skb);
}

void tcp_select_initial_window(uint32_t *rcv_wnd) {
    // recieve window
    *rcv_wnd = 512;
}

int tcp_connect(struct tcp_socket *sock) {
    
    //Transmission Code Block (For Syn/Ack/Wait Requests)
    sock->hln = sizeof(struct tcphdr);
    sock->tcb.iss = generate_iss();
    
    sock->tcb.snd_wnd = 0;
    sock->tcb.snd_wl1 = 0;
    sock->tcb.snd_una = sock->tcb.iss;
    sock->tcb.snd_up  = sock->tcb.iss;
    sock->tcb.snd_nxt = sock->tcb.iss; 
    
    
    sock->tcb.rcv_nxt = 0;
    tcp_select_initial_window(&sock->tcb.rcv_wnd);
    
    
    return tcp_send_syn(sock);
}


