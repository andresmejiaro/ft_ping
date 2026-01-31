#include "ft_ping.h"

volatile sig_atomic_t g_signal_recv = 0;


void signal_handler(int signal){
    g_signal_recv = 1;
    (void) signal;
}


void print_help() {
    printf("Usage: ft_ping [OPTION...] HOST ...\n"
           "Send ICMP ECHO_REQUEST packets to network hosts.\n"
           "\n"
           " Options:\n"
           "\n"
           "  -n                         do not resolve host addresses\n"
           "  -r                         send directly to a host on an attached network\n"
           "  --ttl=N                    specify N as time-to-live\n"
           "  -w                         stop after N seconds\n"
           "  -W                         number of seconds to wait for response\n"
           "  -s                         send NUMBER data octets\n"
           "  -p                         fill ICMP packet with given pattern (hex)\n"
           "\n"
           "  -v                         verbose output\n"
           "  -?                         give this help list\n");
}

int main(int argc, char **argv) {

    params p;


    if (parse_params(argc, argv, &p) != 0) {
        return 1;
    }
    if (p.help_flag) {
        print_help();
        exit(0);
    }
    signal(SIGINT,signal_handler);
    ping(p);
}
