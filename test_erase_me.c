#include "ft_ping.h"

#include <stdio.h>

static void print_params(const params *p) {
    printf("verbose_flag=%d\n", p->verbose_flag);
    printf("help_flag=%d\n", p->help_flag);
    printf("n_flag=%d\n", p->n_flag);
    printf("w_flag=%d\n", p->w_flag);
    printf("w_parameter=%u\n", p->w_parameter);
    printf("W_flag=%d\n", p->W_flag);
    printf("W_parameter=%u\n", p->W_parameter);
    printf("ttl_flag=%d\n", p->ttl_flag);
    printf("ttl_parameter=%u\n", p->ttl_parameter);
}

int main(int argc, char **argv) {
    params p;

    parse_params(argc, argv, &p);
    print_params(&p);
    return 0;
}
