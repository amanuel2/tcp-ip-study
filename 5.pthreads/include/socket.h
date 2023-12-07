#ifndef SOCKET_H_
#define SOCKET_H_

#include "syshead.h"
#include "tcp_socket.h"

int _socket(int domain, int type, int protocol);
int _connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

#endif