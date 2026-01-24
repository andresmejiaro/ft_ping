#include "ft_ping.h"


int main(int argc, char**argv){
    
    params p;

    parse_params(argc, argv, &p);
    
    ping(p);
}


