#include "ft_ping.h"

#include <errno.h>
#include <limits.h>
#include <stdlib.h>

static void init_params(params *p) {
    memset(p, 0, sizeof(*p));
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
        if (parse_uint(value, &p->w_parameter) != 0)
            return -1;
        return 0;
    }
    if (flag == 'W') {
        p->W_flag = 1;
        if (parse_uint(value, &p->W_parameter) != 0)
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

void parse_params(int argc, char **argv, params *parameters) {
    int i;

    init_params(parameters);

    for (i = 1; i < argc; i++) {
        const char *arg = argv[i];

        if (arg[0] != '-' || arg[1] == '\0') {
            if (parameters->addr != NULL) {
                fprintf(stderr, "Error: more than one addr"); // more clonish when advacing
                // free all memory
                exit(1);
            }

            parameters->addr = strdup(arg);

            if (parameters->addr == 0) {
                // cleanup cause malloc failed
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
                    return;
                }
                value = argv[++i];
            } else {
                fprintf(stderr, "unknown option: %s\n", arg);
                return;
            }
            if (set_long_ttl(parameters, value) != 0) {
                fprintf(stderr, "invalid --ttl value: %s\n", value);
                return;
            }
            continue;
        }
        if (arg[1] == '-') {
            fprintf(stderr, "unknown option: %s\n", arg);
            return;
        }
        if (arg[2] != '\0') {
            fprintf(stderr, "unknown option: %s\n", arg);
            return;
        }
        {
            char flag = arg[1];
            const char *value = NULL;

            if (flag == 'w' || flag == 'W' || flag == 's' || flag == 'p') {
                if (i + 1 >= argc) {
                    fprintf(stderr, "missing value for -%c\n", flag);
                    return;
                }
                value = argv[++i];
            }

            if (set_short_flag(parameters, flag, value) != 0) {
                fprintf(stderr, "unknown or invalid option: -%c\n", flag);
                return;
            }
        }
    }
}
