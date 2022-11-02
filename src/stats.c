#include "../inc/ft_ping.h"


void set_rtt_stat(void)
{
    // printf("recv => ");
    long rcv_ms = get_time_from_start();
    (void)rcv_ms;
    // printf("DIFF %ld\n", rcv_ms - g_icmp.stat.send_ms);
}

void print_stats(void)
{
    struct timeval endtime;

    gettimeofday(&endtime, NULL);
    printf("\n--- %s ping statistics ---\n", g_icmp.srvname);
    
    long time_elapsed = (endtime.tv_sec * 1000 + endtime.tv_usec / 1000)- (g_icmp.stat.ping_start.tv_sec * 1000 + g_icmp.stat.ping_start.tv_usec / 1000);
    int loss = (int)((1 - ((float)g_icmp.stat.received / (float)g_icmp.stat.transmitted)) * 100);
    printf("%d packets transmitted, %d received, %d%% packet loss, time %ldms\n", g_icmp.stat.transmitted, g_icmp.stat.received, loss, time_elapsed);
    printf("rtt min/avg/max/mdev = \n");
}