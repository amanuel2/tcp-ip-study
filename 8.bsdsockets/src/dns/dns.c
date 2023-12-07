#include "dns/dns.h"

int get_address(char* host, struct sockaddr* addr) {
    struct addrinfo hints;
    struct addrinfo *servinfo;
    int status;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    
    if((status=getaddrinfo(host, "80", &hints, &servinfo)) != 0) {
        //perror("getaddrinfo: %s\n", gai_strerror(status));
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(1);
    }
    
    // servinfo now points to a linked list of 1 or more struct addrinfos
    struct addrinfo *head;
    for(head=servinfo; head!=0; head = head->ai_next) {
        *addr = *head->ai_addr;
        //freeaddrinfo(servinfo);
        return 0;
    }
    
    return 1;
}
