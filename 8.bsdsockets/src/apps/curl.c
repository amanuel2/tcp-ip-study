#include "lib/syshead.h"
#include "apps/curl.h"
#include "lib/utils.h"

#define MAX_HOSTNAME 50
extern int running;

void* curl(void* arg) {
    struct command *cmd = arg;
    
	if (cmd->argc != 1 || strnlen(cmd->argv[0], MAX_HOSTNAME) == MAX_HOSTNAME) {
		print_error("Curl called but HOST not given or invalid\n");
		return 0;
	}
	
	struct sockaddr addr;
    if(get_address(cmd->argv[0], &addr) != 0)	 {
        perror("Couldnt resolve hostname\n");
        return 0;
    }
    
    printf("%hhu.%hhu.%hhu.%hhu\n", addr.sa_data[2], addr.sa_data[3], addr.sa_data[4], addr.sa_data[5]);
    
    int sockfd = _socket(AF_INET, SOCK_STREAM, 0);
    if(_connect(sockfd, &addr, 6) != 0) {
        print_error("Could not establish connection\n");
        return 0;
    }
    
    running = 0;
    return cmd;
}