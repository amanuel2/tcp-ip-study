#ifndef DNS_H
#define DNS_H

#include "lib/syshead.h"

int get_address(char* host, struct sockaddr* addr);

#endif