#include "ft_ping.h"

int parse_single_param(char *argument, params *parameters){
    if (!strcmp(argument, "-v")){
        parameters->verbose_flag = 1;
       return 0; 
    }
    if (!strcmp(argument, "-?")){
        parameters->help_flag = 1;
        return 0;
    }
    if (!strcmp(argument, "-n")){
        parameters->n_flag = 1;
       return 0;
    } 
    if (!strcmp(argument, "-w")){
        parameters->w_flag = 1;
       return 1;
    }
    if (!strcmp(argument, "-W")){
        parameters->W_flag = 1;
       return 1;
    } 
    if (!strcmp(argument, "-W")){
        parameters->W_flag = 1;
       return 1;
    }
    if (!strcmp(argument, "--ttl")){
        parameters->W_flag = 1;
       return 1;
    } 
    return 2;
}




void parse_params(int argc, char ** argv, params* parameters){



}