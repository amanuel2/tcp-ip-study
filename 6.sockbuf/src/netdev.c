#include "syshead.h"
#include "netdev.h"
#include "ethernet.h"
#include "tuntap_if.h"
#include "basic.h"

struct netdev netdev;
extern int running;

void netdev_init(char *addr, char *hwaddr)
{
    struct netdev *dev = &netdev;
    CLEAR(*dev);
    CLEAR(dev->buf);
    dev->buflen = BUFLEN;

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
 
    dev->tundev = malloc(10);
    tun_init(dev->tundev, addr);
    
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

void netdev_transmit(struct netdev *dev, struct eth_hdr *hdr, 
                     uint16_t ethertype, int len, uint8_t *dst)
{
    uint8_t dst_mac[6];
    memcpy(dst_mac, dst, 6);
    hdr->ethertype = htons(ethertype);

    memcpy(hdr->smac, dev->hwaddr, 6);
    memcpy(hdr->dmac, dst_mac, 6);

    len += sizeof(struct eth_hdr);
    
    // Print the Ethernet header
    printf("Ethernet Header:\n");
    printf("Destination MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
           hdr->dmac[0], hdr->dmac[1], hdr->dmac[2], 
           hdr->dmac[3], hdr->dmac[4], hdr->dmac[5]);
    printf("Source MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
           hdr->smac[0], hdr->smac[1], hdr->smac[2], 
           hdr->smac[3], hdr->smac[4], hdr->smac[5]);
    printf("Ethertype: 0x%04x\n", ntohs(hdr->ethertype));

    tun_write((char *)hdr, len);
}


void* netdev_loop() {
    while (running) {
        if (tun_read(netdev.buf, netdev.buflen) < 0) {
            print_error("ERR: Read from tun_fd: %s\n", strerror(errno));
            return 0; // 1
        }
    
        struct eth_hdr *hdr = init_eth_hdr(netdev.buf);
    
        handle_frame(&netdev, hdr);
    }
    
    return 0;
}

void netdev_free() {
    free(netdev.tundev);
}