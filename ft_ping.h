#ifndef FT_PING_H
 #define FT_PING_H
 #define PAYLOAD_LENGTH 56
 #define _POSIX_C_SOURCE 200112L // for struct addrinfo
 #include <sys/socket.h>
 #include <stdio.h>
 #include <unistd.h>
 #include <strings.h>
 #include <netdb.h>
 #include <string.h>
 #include <netinet/ip_icmp.h>
 #include <stdint.h>


 typedef struct {
    int verbose_flag;
    int help_flag;
    int n_flag;
    int w_flag;
    unsigned int w_parameter;
    int W_flag;
    unsigned int W_parameter;
    int ttl_flag;
    unsigned int ttl_parameter;
    char * addr;
 } params;

 void parse_params(int argc, char **argv, params *parameters);
 void ping(params parameters);
 void build_request(uint8_t *buf);

#endif
