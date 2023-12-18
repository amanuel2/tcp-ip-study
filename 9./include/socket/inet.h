#ifndef INET_H_
#define INET_H_

#include "lib/syshead.h"
#include "socket.h"

int inet_create(struct socket *sock, int protcol);
int inet_socket(struct socket *sock, int protocol);


int inet_connect(struct socket *sock, struct sockaddr *addr, int len, int flags);
int inet_stream_connect(struct socket *sock, struct sockaddr *addr, int len, int flags);


#endif