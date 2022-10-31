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
    struct timeval endtime;
    struct msghdr msg;

    if (recvmsg(icmp.sockfd, &msg, MSG_DONTWAIT))
        icmp.stat.transmitted--;
    
    gettimeofday(&endtime, NULL);
    printf("\n--- %s ping statistics ---\n", icmp.srvname);
    
    long time_elapsed = (endtime.tv_sec * 1000 + endtime.tv_usec / 1000)- (icmp.stat.ping_start.tv_sec * 1000 + icmp.stat.ping_start.tv_usec / 1000);
    int loss = (int)((1 - ((float)icmp.stat.received / (float)icmp.stat.transmitted)) * 100);
    printf("%d packets transmitted, %d received, %d%% packet loss, time %ldms\n", icmp.stat.transmitted, icmp.stat.received, loss, time_elapsed);
    printf("rtt min/avg/max/mdev = \n");
    close(icmp.sockfd);
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

long set_time_from_start(void)
{
    struct timeval now;
    gettimeofday(&now, NULL);
    
    long start_ms = icmp.stat.ping_start.tv_sec * 1000 + icmp.stat.ping_start.tv_usec / 1000;
    long now_ms = now.tv_sec * 1000 + now.tv_usec / 1000;
    printf("ms %ld\n", now_ms - start_ms);
    return now_ms - start_ms;
}