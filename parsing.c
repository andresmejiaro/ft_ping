#include "ft_ping.h"

#include <errno.h>
#include <limits.h>
#include <stdlib.h>

static void init_params(params *p) {
    memset(p, 0, sizeof(*p));
    p->W_parameter = 10.0;
    p->w_parameter = -1.0;
}

static void free_params_local(params *p) {
    free(p->p_parameter);
    free(p->addr);
    p->p_parameter = NULL;
    p->addr = NULL;
}

static int parse_uint(const char *s, unsigned int *out) {
    char *end = NULL;
    unsigned long v;

    if (s == NULL || *s == '\0')
        return -1;
    errno = 0;
    v = strtoul(s, &end, 10);
    if (errno != 0 || end == s || *end != '\0' || v > UINT_MAX)
        return -1;
    *out = (unsigned int)v;
    return 0;
}

static int parse_double(const char *s, double *out) {
    char *end = NULL;
    double v;

    if (s == NULL || *s == '\0')
        return -1;
    errno = 0;
    v = strtod(s, &end);
    if (errno != 0 || end == s || *end != '\0')
        return -1;
    if (v < 0.0)
        return -1;
    *out = v;
    return 0;
}

int double_to_timeval(double seconds, struct timeval *out) {
    int neg = 0;
    double frac;
    time_t sec;
    suseconds_t usec;

    if (out == NULL)
        return -1;
    if (seconds < 0.0) {
        neg = 1;
        seconds = -seconds;
    }
    sec = (time_t)seconds;
    frac = seconds - (double)sec;
    if (frac < 0.0)
        frac = 0.0;
    usec = (suseconds_t)(frac * 1000000.0);
    if (usec >= 1000000) {
        sec += 1;
        usec -= 1000000;
    }
    if (neg) {
        sec = -sec;
        usec = -usec;
    }
    out->tv_sec = sec;
    out->tv_usec = usec;
    return 0;
}

static int set_short_flag(params *p, char flag, const char *value) {
    if (flag == 'v') {
        p->verbose_flag = 1;
        return 0;
    }
    if (flag == '?') {
        p->help_flag = 1;
        return 0;
    }
    if (flag == 'n') {
        p->n_flag = 1;
        return 0;
    }
    if (flag == 'r') {
        p->r_flag = 1;
        return 0;
    }
    if (flag == 'w') {
        p->w_flag = 1;
        if (parse_double(value, &p->w_parameter) != 0)
            return -1;
        return 0;
    }
    if (flag == 'W') {
        p->W_flag = 1;
        if (parse_double(value, &p->W_parameter) != 0)
            return -1;
        return 0;
    }
    if (flag == 's') {
        p->s_flag = 1;
        if (parse_uint(value, &p->s_parameter) != 0)
            return -1;
        return 0;
    }
    if (flag == 'p') {
        p->p_flag = 1;
        if (value == NULL || *value == '\0')
            return -1;
        p->p_parameter = strdup(value);
        if (p->p_parameter == NULL)
            return -1;
        return 0;
    }
    return -1;
}

static int set_long_ttl(params *p, const char *value) {
    p->ttl_flag = 1;
    if (parse_uint(value, &p->ttl_parameter) != 0)
        return -1;
    return 0;
}

int parse_params(int argc, char **argv, params *parameters) {
    int i;

    init_params(parameters);

    for (i = 1; i < argc; i++) {
        const char *arg = argv[i];

        if (arg[0] != '-' || arg[1] == '\0') {
            if (parameters->addr != NULL) {
                fprintf(stderr, "Error: more than one addr");
                free_params_local(parameters);
                exit(1);
            }

            parameters->addr = strdup(arg);

            if (parameters->addr == 0) {
                free_params_local(parameters);
                exit(1);
            }
            continue;
        }
        if (strcmp(arg, "--") == 0) {
            i++;
            break;
        }
        if (strncmp(arg, "--ttl", 5) == 0) {
            const char *value = NULL;
            if (arg[5] == '=') {
                value = arg + 6;
            } else if (arg[5] == '\0') {
                if (i + 1 >= argc) {
                    fprintf(stderr, "missing value for --ttl\n");
                    free_params_local(parameters);
                    return -1;
                }
                value = argv[++i];
            } else {
                fprintf(stderr, "unknown option: %s\n", arg);
                free_params_local(parameters);
                return -1;
            }
            if (set_long_ttl(parameters, value) != 0) {
                fprintf(stderr, "invalid --ttl value: %s\n", value);
                free_params_local(parameters);
                return -1;
            }
            continue;
        }
        if (arg[1] == '-') {
            fprintf(stderr, "unknown option: %s\n", arg);
            free_params_local(parameters);
            return -1;
        }
        if (arg[2] != '\0') {
            fprintf(stderr, "unknown option: %s\n", arg);
            free_params_local(parameters);
            return -1;
        }
        {
            char flag = arg[1];
            const char *value = NULL;

            if (flag == 'w' || flag == 'W' || flag == 's' || flag == 'p') {
                if (i + 1 >= argc) {
                    fprintf(stderr, "missing value for -%c\n", flag);
                    free_params_local(parameters);
                    return -1;
                }
                value = argv[++i];
            }

            if (set_short_flag(parameters, flag, value) != 0) {
                fprintf(stderr, "unknown or invalid option: -%c\n", flag);
                free_params_local(parameters);
                return -1;
            }
        }
    }
    if (parameters->addr == NULL) {
        fprintf(stderr, "missing host\n");
        free_params_local(parameters);
        return -1;
    }
    return 0;
}
