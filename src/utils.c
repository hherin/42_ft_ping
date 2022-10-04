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

    printf("\nICI AFFICHER LES STATS\n");
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
