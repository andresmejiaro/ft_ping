#ifndef FT_PING_H
#define FT_PING_H

#include<sys/socket.h>
#include<stdio.h>
#include<unistd.h>
#include<strings.h>
#include<netdb.h>
#include<string.h>

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



} params;


#endif