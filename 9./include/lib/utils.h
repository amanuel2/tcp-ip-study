#ifndef UTILS_H
#define UTILS_H

#define CMDBUFLEN 100
#include "lib/syshead.h"

#define print_debug(str, ...) \
            printf(str" - %s:%u\n", ##__VA_ARGS__, __FILE__, __LINE__);


int run_cmd(char *cmd, ...);
void print_hexdump(char *str, int len);
void print_error(char *str, ...);
uint16_t checksum(void *addr, int count, int start_sum);


// tcp
uint32_t sum_every_16_bits(void * addr, int count);

//extern struct tcp_socket;
int tcp_v4_checksum(struct sock_buff *skb, uint32_t saddr, uint32_t daddr);
int tcp_udp_checksum(uint32_t saddr, uint32_t daddr, uint8_t proto, uint8_t *data, uint16_t size);


uint32_t parse_ipv4_string(char* addr);
#endif
