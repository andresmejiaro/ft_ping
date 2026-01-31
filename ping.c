#include "ft_ping.h"

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
        // todo clean memory

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
        // todo free memory and fd
        exit(1);
    }
    if (double_to_timeval(parameters->W_parameter, &tv) == 0) {
        if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == -1) {
            printf("Error setting socket timeout");
            exit(1);
        }
        fprintf(stderr, "SO_RCVTIMEO set to %ld.%06ld seconds\n", (long)tv.tv_sec, (long)tv.tv_usec); // DEBUG_PRINT
    }
    if (parameters->ttl_flag) {
        int ttl = (int)parameters->ttl_parameter;
        if (setsockopt(sock, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) == -1) {
            printf("Error setting ttl");
            exit(1);
        }
    }
    return sock;
}

void cleanup() {
    // free ai from resolve address use freeaddrinfo
    // close the raw_socket close(raw_socket)
}

void print_header(struct addrinfo *ai, params *parameters) {
    struct sockaddr_in *dir;
    char addr_ip[16];

    dir = (struct sockaddr_in *)ai->ai_addr;

    if (inet_ntop(AF_INET, &dir->sin_addr, addr_ip, 16) == 0) {
        // todo exit error in this and free on this
        exit(1);
    }
    printf("PING %s (%s): %d data bytes\n", parameters->addr, addr_ip, PAYLOAD_LENGTH);
}

void ping(params parameters) {
    struct addrinfo *ai;
    int raw_socket_fd;
    uint8_t request[sizeof(struct icmphdr) + PAYLOAD_LENGTH];
    uint8_t reply[1000]; // fixed size enough for this
    lineprint linep;

    ai = resolve_address(&parameters);
    print_header(ai, &parameters);

    raw_socket_fd = create_raw_socket(&parameters);
    while (1) {
        bzero(reply, 1000);
        bzero(&linep, sizeof(linep));
        build_request(request, &linep);
        send_request(raw_socket_fd, request, ai);
        gettimeofday(&linep.time_sent, NULL);
        receive_reply(raw_socket_fd, reply, &linep);
        parse_reply(request, reply, &linep); // also prints
        sleep(1);
        /* if (check_for_done()){ */
        /*     break; */
        /* } */
    }
    cleanup();
}
