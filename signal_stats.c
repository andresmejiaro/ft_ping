#include "ft_ping.h"

void stats_update(stats *_stats, int back, float time){

    _stats->packages_sent += 1;
    if(back)
        _stats->packages_recieved += 1;
    else
        return ;
    _stats->cum_time += time;
    _stats->cum_time_sq += time*time;
    if (_stats->min_time == 0 || time < _stats->min_time)
        _stats->min_time = time;
    if (_stats->max_time < time){
        _stats->max_time = time;
    }    
    
}

void print_stats(stats *_stats, params *parameters){
    float packet_loss;
    float std;
    float avg;
    float v;
    
    printf("--- %s ping statistics ---\n", parameters->addr);
    if (_stats->packages_recieved > 0)
        packet_loss =(1.0f - (float)_stats->packages_recieved/_stats->packages_sent)*100.0f;
    else
        packet_loss = 100.0f;
    if (_stats->packages_recieved > 0){
        avg = _stats->cum_time/_stats->packages_recieved;
        v = _stats->cum_time_sq/_stats->packages_recieved - avg * avg;
        if (v>0)
            std =sqrtf(v);
        else
            std = 0;
    }
    else 
        avg = -1;
        
    printf("%d packets transmitted, %d packets received, %.3f%% packet loss\n",
        _stats->packages_sent,
         _stats->packages_recieved,
         packet_loss);
    if (_stats->cum_time > 0)
        printf("round-trip min/avg/max/stddev = %f/%f/%f/%f ms\n",
            _stats->min_time,
            avg,
            _stats->max_time,
            std
        );
}