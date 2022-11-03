#include "../inc/ft_ping.h"

void free_everything(void)
{
    if (g_icmp.lst.next) {
        t_send_list *tmp = g_icmp.lst.next;
        while (tmp) {
            t_send_list *remove = tmp;
            tmp = tmp->next;
            free(remove);
        }
    }
    if (g_icmp.adinfo)
        freeaddrinfo(g_icmp.adinfo);
    if (g_icmp.sockfd > -1)
        close(g_icmp.sockfd);
}

void str_exit_error(const char *s1, const char *s2, int exitcode)
{
    fprintf(stderr, "ping: %s: %s\n", s1, s2);
    free_everything();
    exit(exitcode);
}

void ping_end_signal(int nb)
{
    (void)nb;
    
    print_stats();

    close(g_icmp.sockfd);

    int exitcode = (g_icmp.error || !g_icmp.stat.received) ? 1 : 0;
    exit(exitcode);
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

