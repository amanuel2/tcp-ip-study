#include "syshead.h"
#include "tcp.h"
#include "utils.h"

int run_cmd(char *cmd, ...)
{
    va_list ap;
    char buf[CMDBUFLEN];
    va_start(ap, cmd);
    vsnprintf(buf, CMDBUFLEN, cmd, ap);

    va_end(ap);

    printf("%s\n", buf);

    return system(buf);
}

void print_hexdump(char *str, int len)
{
    printf("Printing hexdump:\n");
    for (int i = 0; i < len; i ++) {
        if (i % 8 == 0) printf("\n");
        printf("%02x ", (unsigned char)str[i]);
    }

    printf("\n");
}

void print_error(char *str, ...)
{
    va_list ap;
    char buf[200];
    va_start(ap, str);
    vsnprintf(buf, 200, str, ap);

    va_end(ap);

    perror(buf);
}

uint32_t sum_every_16_bits(void* addr, int count) {
    /* Compute Internet Checksum for "count" bytes
     *         beginning at location "addr".
     * Taken from https://tools.ietf.org/html/rfc1071
     */
    register uint32_t sum = 0;
    uint16_t* ptr = addr;
    
    while( count > 1 )  {
        /*  This is the inner loop */
        sum += * ptr++;
        count -= 2;
    }

    /*  Add left-over byte, if any */
    if( count > 0 )
        sum += * (uint8_t *) addr;
        
    return sum;
}

uint16_t checksum(void *addr, int count, int start_sum) {

    uint32_t sum = start_sum;
    
    sum += sum_every_16_bits(addr, count);
    
    /*  Fold 32-bit sum to 16 bits */
    while (sum>>16)
        sum = (sum & 0xffff) + (sum >> 16);

    return ~sum;
}


int tcp_checksum(struct iphdr* iphdr, struct tcphdr* tcphdr) {
    
    struct tcpiphdr tcpiphdr;
    int sum = 0;
    int tlen = iphdr->len - iphdr->ihl * 4;
    
    ///// setup tcp-ip hdr
    tcpiphdr.saddr = iphdr->saddr;
    tcpiphdr.daddr = iphdr->daddr;
    tcpiphdr.zero = 0;
    tcpiphdr.proto = iphdr->proto;
    tcpiphdr.tlen = htons(tlen);
    
    sum = sum_every_16_bits(&tcpiphdr, sizeof(struct tcpiphdr));
    return checksum(tcphdr, tlen, sum);
    
}

int get_address(char* host, struct sockaddr* addr) {
    struct addrinfo hints;
    struct addrinfo *servinfo;
    int status;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    
    if((status=getaddrinfo(host, NULL, &hints, &servinfo)) != 0) {
        //perror("getaddrinfo: %s\n", gai_strerror(status));
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(1);
    }
    
    // servinfo now points to a linked list of 1 or more struct addrinfos
    struct addrinfo *head;
    for(head=servinfo; head!=0; head = head->ai_next) {
        *addr = *head->ai_addr;
        freeaddrinfo(servinfo);
        return 0;
    }
    
    return 1;
}