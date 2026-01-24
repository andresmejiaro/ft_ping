#include "ft_ping.h"


struct addrinfo* resolve_address(params *parameters){
    struct addrinfo hints;
    struct addrinfo *ai;


    bzero(&hints,sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;

    int err = getaddrinfo(parameters->addr,NULL ,&hints, &ai);
    if (err){
        printf("getaddrinfo: error");
        //todo clean memory
        
        exit(1);
    }

    return ai;
}

int create_raw_socket(){
    int sock;
    sock = socket(AF_INET,SOCK_RAW,IPPROTO_ICMP);
    if (sock == -1){
        printf("Error creating socket");
        //todo free memory and fd
        exit(1);
    }
    return sock;
}



void cleanup(){
    //free ai from resolve address use freeaddrinfo
    //close the raw_socket close(raw_socket)
}

void ping(params parameters){
    struct addrinfo *ai;
    int raw_socket_fd;
    uint8_t request[sizeof(struct icmphdr) + PAYLOAD_LENGTH];
 //fixed size enough for this
    
    
    
    ai = resolve_address(&parameters);
    raw_socket_fd = create_raw_socket();
    while(1){
        build_request(request);
        send_request(raw_socket_fd,request, ai);
        while (!reply_ready()){
            (void)parameters;
        }
        parse_reply();
        print_screen();
        usleep(1);
        if (check_for_done()){
            print_summary();
            break;
        }
    }
    cleanup();

}