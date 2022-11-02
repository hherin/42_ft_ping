#include "../inc/ft_ping.h"

void str_exit_error(char *str)
{
    ft_putstr_fd("ping: usage error: ", 2);
    ft_putstr_fd(str, 2);
    exit(1);
}

void ping_end_signal(int nb)
{
    (void)nb;
    

    // if (recvmsg(g_icmp.sockfd, &msg, MSG_DONTWAIT))
    //     g_icmp.stat.transmitted--;
    
    print_stats();

    close(g_icmp.sockfd);
    exit(1);
}

USHORT CheckSum(UCHAR *msg, int len)
{
    uint32_t sum = 0;

    while (len >= 2) {
        sum += *(uint16_t*)msg;
        msg += 2;
        len -= 2;
    }
    if (len >= 1)
        sum += *msg;
    sum = (sum & 0xffff) + (sum >> 16);
    sum = (sum & 0xffff) + (sum >> 16);
    return ~sum;
}


long get_time_from_start(void)
{
    struct timeval now;
    gettimeofday(&now, NULL);
    
    long start_ms = g_icmp.stat.ping_start.tv_sec * 1000 + g_icmp.stat.ping_start.tv_usec / 1000;
    long now_ms = now.tv_sec * 1000 + now.tv_usec / 1000;
    // printf("ms %ld\n", now_ms - start_ms);
    return now_ms - start_ms;
}
