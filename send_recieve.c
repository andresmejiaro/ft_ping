#include "ft_ping.h"

void send_request(int raw_socket_fd, uint8_t * request, struct addrinfo *ai){
    ssize_t sent_bytes;

    sent_bytes = sendto(raw_socket_fd,request,sizeof(struct icmphdr) + PAYLOAD_LENGTH,0,ai->ai_addr, ai->ai_addrlen); 
    if (sent_bytes < 0){
        // todo free everything
        printf("error sending");
        exit(1);
    }
}

int reply_ready(int raw_socket_fd){
    
    char buffer[1000];
    int recv_bytes;

  
    recv_bytes = recvfrom(raw_socket_fd, buffer,1000,0,NULL, NULL);
    if (recv_bytes < 0){
        // todo free everything
        printf("error sending");
        exit(1);
    }
}