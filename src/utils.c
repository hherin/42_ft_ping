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