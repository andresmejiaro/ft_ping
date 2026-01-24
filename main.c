#include "ft_ping.h"


int main(int argc, char**argv){
    struct addrinfo hints = {0}, *ai;
    
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;
    
    int err = getaddrinfo("www.google.com",NULL,&hints,&ai);    
    if (err)
        _exit(1);


    int sock_fd = socket(AF_INET,SOCK_RAW, IPPROTO_ICMP); // 1 is ICMP Intercet control message protocol man socket man protocol
    
    printf("This is my little socket %d", sock_fd);
    close(sock_fd);
    return 0;
    (void) argc;
    (void) argv;
}


