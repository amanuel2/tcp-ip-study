#include "lib/syshead.h"
#include "lib/utils.h"
#include "lib/basic.h"

static int tun_fd;
static char* dev;

char *tap_addr = "10.0.0.5/24";
char *tap_route = "10.0.0.0/24";

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
    printf("WRITING %s\n", buf);
    return write(tun_fd, buf, len);
}

void tun_init() {
    
    dev = calloc(10,1);
    tun_fd = tun_alloc(dev);

    if (set_if_up(dev) != 0) {
        print_error("ERROR when setting up if\n");
    }

    if (set_if_route(dev, tap_route)!= 0) { // Adjust the subnet mask as needed
        print_error("ERROR when setting route for if\n");
    }

    if (set_if_address(dev, tap_addr) != 0) { // Use CIDR notation
        print_error("ERROR when setting addr for if\n");
    }
}


void tun_free() { free(dev); }