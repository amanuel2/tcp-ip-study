#include "lib/syshead.h"
#include "tcp/tcp.h"
#include "lib/utils.h"

int run_cmd(char *cmd, ...) {
    va_list ap;
    char buf[CMDBUFLEN];
    va_start(ap, cmd);
    vsnprintf(buf, CMDBUFLEN, cmd, ap);

    va_end(ap);

    printf("%s\n", buf);

    return system(buf);
}

void print_hexdump(char *str, int len) {
    printf("Printing hexdump:\n");
    for (int i = 0; i < len; i ++) {
        if (i % 8 == 0) printf("\n");
        printf("%02x ", (unsigned char)str[i]);
    }
    printf("\n");
}

void print_error(char *str, ...) {
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


int tcp_udp_checksum(uint32_t saddr, uint32_t daddr, 
                            uint8_t proto,
                            uint8_t *data, uint16_t size) {
    
    uint32_t sum = 0;

    sum += saddr;
    sum += daddr;
    sum += htons(proto);
    sum += htons(size);
    
    return checksum(data, size, sum);
}

int tcp_v4_checksum(struct sock_buff *skb, uint32_t saddr, uint32_t daddr) {

    return tcp_udp_checksum(saddr, daddr, IP_TCP, skb->data, skb->size);
    
}


uint32_t parse_ipv4_string(char* addr) {
    uint8_t bytes[4];
    sscanf(addr, "%hhu.%hhu.%hhu.%hhu", &bytes[3], &bytes[2], &bytes[1], &bytes[0]);
    return bytes[0] | bytes[1] << 8 | bytes[2] << 16 | bytes[3] << 24;
}