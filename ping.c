#include "ft_ping.h"
void free_params(params *pars){
    free(pars->p_parameter);
    free(pars->addr);

}


void cleanup(struct addrinfo *ai, int socket_fd, params *pars) {
    freeaddrinfo(ai);
    close(socket_fd);
    free_params(pars);
}

struct addrinfo *resolve_address(params *parameters) {
    struct addrinfo hints;
    struct addrinfo *ai;

    bzero(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_protocol = IPPROTO_ICMP;

    int err = getaddrinfo(parameters->addr, NULL, &hints, &ai);
    if (err) {
        printf("getaddrinfo: error");
        free_params(parameters);
        exit(1);
    }

    return ai;
}

int create_raw_socket(params *parameters) {
    int sock;
    struct timeval tv;

    sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock == -1) {
        printf("Error creating socket");
        free_params(parameters);
        exit(1);
    }
    if (double_to_timeval(parameters->W_parameter, &tv) == 0) {
        if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == -1) {
            printf("Error setting socket timeout");
            free_params(parameters);
            close(sock);
            exit(1);
        }
    }
    if (parameters->ttl_flag) {
        int ttl = (int)parameters->ttl_parameter;
        if (setsockopt(sock, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) == -1) {
            printf("Error setting ttl");
            free_params(parameters);
            close(sock);
            exit(1);
        }
    }
    return sock;
}


void print_header(struct addrinfo *ai, params *parameters) {
    struct sockaddr_in *dir;
    char addr_ip[16];

    dir = (struct sockaddr_in *)ai->ai_addr;

    if (inet_ntop(AF_INET, &dir->sin_addr, addr_ip, 16) == 0) {
        strcpy(addr_ip,"Unknown");
    }
    printf("PING %s (%s): %d data bytes\n", parameters->addr, addr_ip, PAYLOAD_LENGTH);
}

void ping(params parameters) {
    struct addrinfo *ai;
    int raw_socket_fd;
    uint8_t request[sizeof(struct icmphdr) + PAYLOAD_LENGTH];
    uint8_t reply[1000]; // fixed size enough for this
    lineprint linep;
    stats ping_stats;
    struct timeval start_time, end_time;
    

    ai = resolve_address(&parameters);
    print_header(ai, &parameters);

    raw_socket_fd = create_raw_socket(&parameters);
    bzero(&ping_stats, sizeof(stats));
    gettimeofday(&start_time, NULL); 
    while (1) {
        bzero(reply, 1000);
        bzero(&linep, sizeof(linep));
        build_request(request, &linep);
        if (send_request(raw_socket_fd, request, ai)){
            cleanup(ai,raw_socket_fd,&parameters);
            exit(1);
        }
        gettimeofday(&linep.time_sent, NULL);
        if (receive_reply(raw_socket_fd, reply, &linep)){
            cleanup(ai,raw_socket_fd,&parameters);
            exit(1);
        }
        if (linep.received_size > 0 && parse_reply(request, reply, &linep)){
            linep.received_size = -1;
        } 
        stats_update(&ping_stats,linep.received_size >= 0,linep.time_diff);
        if (g_signal_recv){ 
             break;
        }
        gettimeofday(&end_time,NULL);
        if (parameters.w_parameter >= 0 && timediff(end_time,start_time) > parameters.w_parameter*1000){
            break;
        }
        sleep(1);
    }
    print_stats(&ping_stats,&parameters);
    cleanup(ai,raw_socket_fd,&parameters);
}
