#ifndef FT_PING_H
#define FT_PING_H
#define _DEFAULT_SOURCE
#define PAYLOAD_LENGTH 56
#define _POSIX_C_SOURCE 200112L // for struct addrinfo
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct {
    int verbose_flag;
    int help_flag;
    int n_flag;
    int r_flag;
    int w_flag;
    unsigned int w_parameter;
    int W_flag;
    unsigned int W_parameter;
    int s_flag;
    unsigned int s_parameter;
    int p_flag;
    char *p_parameter;
    int ttl_flag;
    unsigned int ttl_parameter;
    char *addr;
} params;

typedef struct {
    int received_size;
    char address[20];
    int icmp_seq_n;
    int ttl;
    struct timeval time_sent;
    struct timeval time_recieved;
    float time_diff;
} lineprint;

void parse_params(int argc, char **argv, params *parameters);
void ping(params parameters);
void build_request(uint8_t *buf, lineprint *);
void send_request(int raw_socket_fd, uint8_t *request, struct addrinfo *ai);
void receive_reply(int raw_socket_fd, uint8_t *reply, lineprint *);
int parse_reply(uint8_t *request, uint8_t *reply, lineprint *);

#endif
