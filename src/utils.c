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
    printf("\n--- %s ping statistics ---\n", icmp.srvname);
    
    struct timeval endtime;
    gettimeofday(&endtime, NULL);
    long time_elapsed = (endtime.tv_sec * 1000 + endtime.tv_usec / 1000)- (icmp.stat.time.tv_sec * 1000 + icmp.stat.time.tv_usec / 1000);
    int loss = (!icmp.stat.received) ? 100.0 : (int)(icmp.stat.transmitted / icmp.stat.received);
    printf("%d packets transmitted, %d received, %d%% packet loss, time %ldms\n", icmp.stat.transmitted, icmp.stat.received, loss, time_elapsed);
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

void my_sleep(int nb)
{
    struct timeval time;
    gettimeofday(&time, NULL);
    while (1) {
        struct timeval tmp;
        gettimeofday(&tmp, NULL);
        if (tmp.tv_sec - time.tv_sec >= nb)
            break;
    }
}