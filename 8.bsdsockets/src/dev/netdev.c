#include "lib/syshead.h"
#include "dev/netdev.h"
#include "eth/eth.h"
#include "dev/tuntap_if.h"
#include "lib/basic.h"

struct netdev netdev;
extern int running;

void netdev_init(char *addr, char *hwaddr)
{
    struct netdev *dev = &netdev;
    CLEAR(*dev);

    if (inet_pton(AF_INET, addr, &dev->addr) != 1) {
        perror("ERR: Parsing inet address failed\n");
        exit(1);
    }

    sscanf(hwaddr, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &dev->hwaddr[0],
                                                    &dev->hwaddr[1],
                                                    &dev->hwaddr[2],
                                                    &dev->hwaddr[3],
                                                    &dev->hwaddr[4],
                                                    &dev->hwaddr[5]);
    
    dev->addr_len = 6;
     
    //dev->tundev = malloc(10); 
    
    tun_init();
    
    // Print the IP address
    printf("Network Device Initalized\n");
    printf("-------------------------\n");
    char str_addr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &dev->addr, str_addr, INET_ADDRSTRLEN);
    printf("IP Address: %s\n", str_addr);

    // Print the MAC address
    printf("MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
           dev->hwaddr[0], dev->hwaddr[1], dev->hwaddr[2],
           dev->hwaddr[3], dev->hwaddr[4], dev->hwaddr[5]);
}


int netdev_queue_xmit(struct sock_buff *skb) {
    
}

int netdev_transmit(struct sock_buff *skb, uint16_t ethertype, uint8_t *dst_hw) {
    
    struct netdev *dev;
    struct eth_hdr *hdr;
    int rc = 0;

    dev = skb->netdev;

    skb_push(skb, ETH_HDR_LEN);

    hdr = (struct eth_hdr *)skb->data;

    memcpy(hdr->dmac, dst_hw, dev->addr_len);
    memcpy(hdr->smac, dev->hwaddr, dev->addr_len);
    hdr->ethertype = htons(ethertype);

    rc = tun_write((char *)skb->data, skb->size);

    free_skb(skb);

    return rc;
    
}


struct netdev* netdev_get(uint32_t sip) {
    if(netdev.addr == sip)   return &netdev;
    return NULL;
}




int netdev_rx_action(struct sock_buff *skb) {
    struct eth_hdr *hdr = eth_hdr(skb);
    switch (hdr->ethertype) {
        case ETH_P_ARP:
            printf("ETH ARP RCV (Either an ARP Req or ARP Reply)\n");
            arp_rcv(skb);
            break;
        case ETH_P_IP:
            printf("ETH IP REQ\n");
            ip_rcv(skb);
            break;
        case ETH_P_IPV6:
            //printf("IPV6 Isn't implemented yet\n");
            break;
        default:
            printf("Unrecognized ethertype %x\n", hdr->ethertype);
            break;
    }
    
    return 0;
}

void* netdev_loop() {
    while (running) {
        struct sock_buff *skb = alloc_skb(BUFLEN);
        printf("--STARTING READ--\n");
        int res = tun_read((char*) skb->data, BUFLEN);
        printf("RES:%d, BUFF:%s\n", res, skb->data);
        if (res < 0) {
            print_error("ERR: Read from tun_fd: %s\n", strerror(errno));
            return 0; // 1
        }
        
        printf(" <<< ---- PACKETS COMING TO US --- <<< \n");
        netdev_rx_action(skb);
    }
    
    return 0;
}

void netdev_free() {
}