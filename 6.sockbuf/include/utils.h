#ifndef UTILS_H
#define UTILS_H

#define CMDBUFLEN 100
#include "syshead.h"

int run_cmd(char *cmd, ...);
void print_hexdump(char *str, int len);
void print_error(char *str, ...);
uint16_t checksum(void *addr, int count, int start_sum);


// tcp
uint32_t sum_every_16_bits(void * addr, int count);

//extern struct tcp_socket;
int tcp_checksum(struct tcp_socket *sock, struct tcphdr* tcphdr);

// curl
int get_address(char* host, struct sockaddr* addr);

uint32_t parse_ipv4_string(char* addr);
#endif
