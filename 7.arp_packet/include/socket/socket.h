#ifndef SOCKET_H_
#define SOCKET_H_

#include "lib/syshead.h"
#include "tcp/tcp.h"

int _socket(int domain, int type, int protocol);
int _connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

#endif