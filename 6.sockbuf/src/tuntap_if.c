#include "syshead.h"
#include "utils.h"
#include "basic.h"

static int tun_fd;

static int set_if_route(char *dev, char *cidr)
{
    return run_cmd("ip route add dev %s %s", dev, cidr);
}

static int set_if_address(char *dev, const char *cidr)
{
    return run_cmd("ip address add dev %s local %s", dev, cidr);
    //return run_cmd("ip address add %s dev %s", cidr, dev);
}

static int set_if_up(char *dev)
{
    return run_cmd("ip link set dev %s up", dev);
}

/*
 * Taken from Kernel Documentation/networking/tuntap.txt
 */
static int tun_alloc(char *dev)
{
    struct ifreq ifr;
    int fd, err;

    if( (fd = open("/dev/net/tap", O_RDWR)) < 0 ) {
        print_error("Cannot open TUN/TAP dev\n"
                    "Make sure one exists with " 
                    "'$ mknod /dev/net/tap c 10 200'\n");
        exit(1);
    }

    CLEAR(ifr);

    /* Flags: IFF_TUN   - TUN device (no Ethernet headers)
     *        IFF_TAP   - TAP device
     *
     *        IFF_NO_PI - Do not provide packet information
     */
    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
    if( *dev ) {
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

    if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ){
        print_error("ERR: Could not ioctl tun: %s\n", strerror(errno));
        close(fd);
        return err;
    }

    strcpy(dev, ifr.ifr_name);
    return fd;
}

int tun_read(char *buf, int len)
{
    return read(tun_fd, buf, len);
}

int tun_write(char *buf, int len)
{
    return write(tun_fd, buf, len);
}

void tun_init(char *dev, char* addr) {
    
    tun_fd = tun_alloc(dev);

    if (set_if_up(dev) != 0) {
        print_error("ERROR when setting up if\n");
    }

    if (set_if_route(dev, "10.0.0.0/24") != 0) { // Adjust the subnet mask as needed
        print_error("ERROR when setting route for if\n");
    }


//    char full_addr = malloc(strlen(addr) + 4 + 1);
//    if (full_addr == NULL) {
    // Handle memory allocation failure
//    perror("Failed to allocate memory");
//    exit(EXIT_FAILURE);
//}
//strcpy(full_addr, addr); // Copy the first string into full_addr
//strcat(full_addr, "/24"); // Concatenate "/24" to the end
    
    if (set_if_address(dev, "10.0.0.5/24") != 0) { // Use CIDR notation
        print_error("ERROR when setting addr for if\n");
    }
}