#include "ft_ping.h"
#include <errno.h>

void send_request(int raw_socket_fd, uint8_t *request, struct addrinfo *ai) {
    ssize_t sent_bytes;

    sent_bytes = sendto(raw_socket_fd, request, sizeof(struct icmphdr) + PAYLOAD_LENGTH, 0,
                        ai->ai_addr, ai->ai_addrlen);
    if (sent_bytes < 0) {
        // todo free everything
        printf("error sending");
        exit(1);
    }
}

void receive_reply(int raw_socket_fd, uint8_t *reply, lineprint *linep) {

    int recv_bytes;

    /* TODO: add timeout (SO_RCVTIMEO or select/poll) to avoid blocking forever */
    recv_bytes = recvfrom(raw_socket_fd, reply, 1000, 0, NULL, NULL);
    if (recv_bytes < 0) {
        fprintf(stderr, "recvfrom error errno=%d (%s)\n", errno, strerror(errno)); // DEBUG_PRINT
        // todo free everything
        printf("error recieving");
        exit(1);
    }
    gettimeofday(&linep->time_recieved, NULL);
    linep->received_size = recv_bytes;
}

struct icmphdr *align_header(uint8_t *buffer) {
    struct ip *ip_pointer;
    struct icmphdr *icmp;
    int ip_hlen;

    ip_pointer = (struct ip *)buffer;
    ip_hlen = ip_pointer->ip_hl * 4;
    icmp = (struct icmphdr *)(buffer + ip_hlen);

    return icmp;
}

int validate_reply(struct icmphdr *icmp_reply, struct icmphdr *icmp_request) {
    int cond1, cond2, cond3;

    cond1 = (icmp_reply->type == ICMP_ECHOREPLY);
    cond2 = (icmp_reply->un.echo.id == icmp_request->un.echo.id);
    cond3 = (icmp_reply->un.echo.sequence == icmp_request->un.echo.sequence);

    if (cond1 && cond2 && cond3)
        return 1;
    return 0;
}

float timediff(struct timeval tim1, struct timeval tim2) {
    long w;

    w = 1000000 * (tim1.tv_sec - tim2.tv_sec) + tim1.tv_usec - tim2.tv_usec;
    return ((float)w / 1000);
}

int parse_reply(uint8_t *request, uint8_t *reply, lineprint *linep) {

    struct icmphdr *icmp_reply, *icmp_request;
    struct ip *reply_ip;

    icmp_reply = align_header(reply);
    icmp_request = (struct icmphdr *)request;
    reply_ip = (struct ip *)reply;

    if (inet_ntop(AF_INET, &reply_ip->ip_src, linep->address, 16) == 0) {
        // todo exit error in this and free on this
        exit(1);
    }
    linep->received_size -= reply_ip->ip_hl * 4;
    linep->ttl = reply_ip->ip_ttl;
    linep->time_diff = timediff(linep->time_recieved, linep->time_sent);

    if (validate_reply(icmp_reply, icmp_request)) {
        printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n", linep->received_size,
               linep->address, linep->icmp_seq_n, linep->ttl, linep->time_diff);

        return 0;
    } else {
        return 1;
    }
}
